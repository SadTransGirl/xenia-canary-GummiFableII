/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2022 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/kernel/xevent.h"

#include "xenia/base/byte_stream.h"
#include "xenia/base/logging.h"

namespace xe {
namespace kernel {

XEvent::XEvent(KernelState* kernel_state)
    : XObject(kernel_state, kObjectType) {}

XEvent::~XEvent() = default;

void XEvent::Initialize(bool manual_reset, bool initial_state) {
  assert_false(event_);

  this->CreateNative<X_KEVENT>();

  if (manual_reset) {
    event_ = xe::threading::Event::CreateManualResetEvent(initial_state);
  } else {
    event_ = xe::threading::Event::CreateAutoResetEvent(initial_state);
  }
  assert_not_null(event_);
}

void XEvent::InitializeNative(void* native_ptr, X_DISPATCH_HEADER* header) {
  assert_false(event_);

  switch (header->type) {
    case 0x00:  // EventNotificationObject (manual reset)
      manual_reset_ = true;
      break;
    case 0x01:  // EventSynchronizationObject (auto reset)
      manual_reset_ = false;
      break;
    default:
      assert_always();
      return;
  }

  bool initial_state = header->signal_state ? true : false;
  if (manual_reset_) {
    event_ = xe::threading::Event::CreateManualResetEvent(initial_state);
  } else {
    event_ = xe::threading::Event::CreateAutoResetEvent(initial_state);
  }
  assert_not_null(event_);
}

void XEvent::SyncGuestSignalState(uint32_t state) {
  // Real Xenon keeps the KEVENT dispatcher header's signal_state accurate.
  // Xenia only manipulated the host event, leaving the guest field stale at its
  // KeInitializeEvent value - so guest worker loops that poll the header
  // directly (a common hand-rolled sync pattern) never saw a Set and could
  // Reset+Wait past it, losing the wakeup (Fable II worker-pool deadlock).
  if (guest_object()) {
    guest_object<X_DISPATCH_HEADER>()->signal_state = state;
  }
}

int32_t XEvent::Set(uint32_t priority_increment, bool wait) {
  set_priority_increment(priority_increment);
  DbgRecordSignal(1);
  SyncGuestSignalState(1);
  event_->Set();
  return 1;
}

int32_t XEvent::Pulse(uint32_t priority_increment, bool wait) {
  set_priority_increment(priority_increment);
  DbgRecordSignal(2);
  // Pulse releases waiters then leaves the event non-signaled.
  SyncGuestSignalState(0);
  event_->Pulse();
  return 1;
}

int32_t XEvent::Reset() {
  DbgRecordSignal(3);
  SyncGuestSignalState(0);
  event_->Reset();
  return 1;
}

void XEvent::WaitCallback() {
  // Auto-reset events clear their signal on a satisfied wait; keep the guest
  // header in sync (manual-reset events stay signaled until an explicit Reset).
  if (!manual_reset_) {
    SyncGuestSignalState(0);
  }
}
void XEvent::Query(uint32_t* out_type, uint32_t* out_state) {
  auto [type, state] = event_->Query();

  *out_type = type;
  *out_state = state;
}
void XEvent::Clear() { event_->Reset(); }

bool XEvent::Save(ByteStream* stream) {
  XELOGD("XEvent {:08X} ({})", handle(), manual_reset_ ? "manual" : "auto");
  SaveObject(stream);

  bool signaled = true;
  auto result =
      xe::threading::Wait(event_.get(), false, std::chrono::milliseconds(0));
  if (result == xe::threading::WaitResult::kSuccess) {
    signaled = true;
  } else if (result == xe::threading::WaitResult::kTimeout) {
    signaled = false;
  } else {
    assert_always();
  }

  if (signaled) {
    // Reset the event in-case it's an auto-reset.
    event_->Set();
  }

  stream->Write<bool>(signaled);
  stream->Write<bool>(manual_reset_);

  return true;
}

object_ref<XEvent> XEvent::Restore(KernelState* kernel_state,
                                   ByteStream* stream) {
  auto evt = new XEvent(nullptr);
  evt->kernel_state_ = kernel_state;

  evt->RestoreObject(stream);
  bool signaled = stream->Read<bool>();
  evt->manual_reset_ = stream->Read<bool>();

  if (evt->manual_reset_) {
    evt->event_ = xe::threading::Event::CreateManualResetEvent(false);
  } else {
    evt->event_ = xe::threading::Event::CreateAutoResetEvent(false);
  }
  assert_not_null(evt->event_);

  if (signaled) {
    evt->event_->Set();
  }

  return object_ref<XEvent>(evt);
}

}  // namespace kernel
}  // namespace xe
