// A Status encapsulates the result of an operation.  It may indicate success,
// or it may indicate an error with an associated error message.
//
// Multiple threads can invoke const methods on a Status without
// external synchronization, but if any of the threads may call a
// non-const method, all threads accessing the same Status must use
// external synchronization.

#ifndef STATUS_H_
#define STATUS_H_

#include <string>
#include "slice.h"

#ifdef _WIN32
#if _MSC_VER < 1500				// 1500是VC9，也就是VS2008
#define deleteConstArray(ptr) do{delete [] const_cast<char*> (ptr);}while(0)
#endif
#endif

#ifndef deleteConstArray
#define deleteConstArray(ptr) do{delete [] ptr;}while(0)
#endif 




class Status {
  public:
    // Create a success status.
    Status() :
        state_(NULL) {
    }
    ~Status() {
      deleteConstArray(state_);
    }

    // Copy the specified status.
    Status(const Status& s);
    void operator=(const Status& s);

    // Return a success status.
    static Status OK() {
      return Status();
    }

    // Return error status of an appropriate type.
    static Status ObjNotFound(const Slice& msg, const Slice& msg2 = Slice()) {
      return Status(kObjNotFound, msg, msg2);
    }

    static Status Corruption(const Slice& msg, const Slice& msg2 = Slice()) {
      return Status(kCorruption, msg, msg2);
    }

    static Status NotSupported(const Slice& msg, const Slice& msg2 = Slice()) {
      return Status(kNotSupported, msg, msg2);
    }

    static Status InvalidArgument(const Slice& msg, const Slice& msg2 =
                                      Slice()) {
      return Status(kInvalidArgument, msg, msg2);
    }

    static Status IOError(const Slice& msg, const Slice& msg2 = Slice()) {
      return Status(kIOError, msg, msg2);
    }

    static Status IOTimeout(const Slice& msg = Slice(), const Slice& msg2 =
                                Slice()) {
      return Status(kIOTimeout, msg, msg2);
    }

    static Status ObjExist(const Slice& msg, const Slice& msg2 = Slice()) {
      return Status(kObjExist, msg, msg2);
    }

    static Status ChildrenExist(const Slice& msg, const Slice& msg2 = Slice()) {
      return Status(kChildrenExist, msg, msg2);
    }

    static Status ObjFinalized(const Slice& msg, const Slice& msg2 = Slice()) {
      return Status(kObjFinalized, msg, msg2);
    }

    static Status ObjNotFinalized(const Slice& msg, const Slice& msg2 =
                                      Slice()) {
      return Status(kObjNotFinalized, msg, msg2);
    }

    static Status DataMissing(const Slice& msg, const Slice& msg2 = Slice()) {
      return Status(kDataMissing, msg, msg2);
    }

    static Status EndOfFile(const Slice& msg, const Slice& msg2 = Slice()) {
      return Status(kEndOfFile, msg, msg2);
    }

    static Status TooManyItems() {
      return Status(kTooManyItems, "", "");
    }

    static Status BlobTooLarge() {
      return Status(kBlobTooLarge, "", "");
    }

    static Status AttachmentTooLarge(const Slice& msg, const Slice& msg2 =
                                         Slice()) {
      return Status(kAttachmentTooLarge, msg, msg2);
    }

    static Status NoQuota(const Slice& msg, const Slice& msg2 = Slice()) {
      return Status(KNoQuota, msg, msg2);
    }

    static Status NoDiskSpace(const Slice& msg, const Slice& msg2 = Slice()) {
      return Status(kNoDiskSpace, msg, msg2);
    }

    static Status OutOfPower(const Slice& msg, const Slice& msg2 = Slice()) {
      return Status(kOutOfPower, msg, msg2);
    }

    static Status LostMetaConnection(const Slice& msg, const Slice& msg2 =
                                         Slice()) {
      return Status(kLostMetaConnection, msg, msg2);
    }

    static Status LostCsConnection(const Slice& msg, const Slice& msg2 =
                                       Slice()) {
      return Status(kLostCsConnection, msg, msg2);
    }

    static Status WriteBufferFull(const Slice& msg, const Slice& msg2 =
                                      Slice()) {
      return Status(kWriteBufferFull, msg, msg2);
    }

// Returns true iff the status indicates success.
    bool ok() const {
      return (state_ == NULL);
    }

// Returns true iff the status indicates a NotFound error.
    bool IsNotFound() const {
      return code() == kObjNotFound;
    }

    bool IsExist() const {
      return code() == kObjExist;
    }

    bool IsFinalized() const {
      return code() == kObjFinalized;
    }

// Returns true iff the status indicates a Corruption error.
    bool IsCorruption() const {
      return code() == kCorruption;
    }

// Returns true iff the status indicates an IOError.
    bool IsIOError() const {
      return code() == kIOError;
    }

    bool IsIOTimeout() const {
      return code() == kIOTimeout;
    }

    bool IsEndOfFile() const {
      return code() == kEndOfFile;
    }

    bool IsTooManyItems() const {
      return code() == kTooManyItems;
    }

    bool IsBlobTooLarge() const {
      return code() == kBlobTooLarge;
    }

    bool IsAttachmentTooLarge() const {
      return code() == kAttachmentTooLarge;
    }

    bool IsDataMissing() const {
      return code() == kDataMissing;
    }

    bool IsChildrenExist() const {
      return code() == kChildrenExist;
    }

    bool IsNoQuota() const {
      return code() == KNoQuota;
    }

    bool IsNoDiskSpace() const {
      return code() == kNoDiskSpace;
    }

    bool IsOutOfPower() const {
      return code() == kOutOfPower;
    }

    bool IsLostMetaConnection() const {
      return code() == kLostMetaConnection;
    }

    bool IsLostCsConnection() const {
      return code() == kLostCsConnection;
    }

    bool IsWriteBufferFull() const {
      return code() == kWriteBufferFull;
    }

// Return a string representation of this status suitable for printing.
// Returns the string "OK" for success.
    std::string ToString() const;

  private:
// OK status has a NULL state_.  Otherwise, state_ is a new[] array
// of the following form:
//    state_[0..3] == length of message
//    state_[4]    == code
//    state_[5..]  == message
    const char* state_;

    enum Code {
      kOk = 0,
      kObjNotFound,
      kObjExist,
      kObjFinalized,
      kObjNotFinalized,
      kCorruption,
      kChildrenExist,
      kDataMissing,
      kEndOfFile,
      kTooManyItems,
      kBlobTooLarge,
      kAttachmentTooLarge,
      kNotSupported,
      kInvalidArgument,
      kIOError,
      kIOTimeout,
      KNoQuota,
      kNoDiskSpace,
      kOutOfPower,
      kLostMetaConnection,
      kLostCsConnection,
      kWriteBufferFull
    };

    Code code() const {
      return (state_ == NULL) ? kOk : static_cast<Code>(state_[4]);
    }

    Status(Code code, const Slice& msg, const Slice& msg2);
    static const char* CopyState(const char* s);
};

inline Status::Status(const Status& s) {
  state_ = (s.state_ == NULL) ? NULL : CopyState(s.state_);
}
inline void Status::operator=(const Status& s) {
// The following condition catches both aliasing (when this == &s),
// and the common case where both s and *this are ok.
  if (state_ != s.state_) {
    deleteConstArray(state_);
    state_ = (s.state_ == NULL) ? NULL : CopyState(s.state_);
  }
}

#endif  // STATUS_H_
