// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: person.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_person_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_person_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3008000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3008000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_person_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_person_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_person_2eproto;
class person;
class personDefaultTypeInternal;
extern personDefaultTypeInternal _person_default_instance_;
PROTOBUF_NAMESPACE_OPEN
template<> ::person* Arena::CreateMaybeMessage<::person>(Arena*);
PROTOBUF_NAMESPACE_CLOSE

// ===================================================================

class person :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:person) */ {
 public:
  person();
  virtual ~person();

  person(const person& from);
  person(person&& from) noexcept
    : person() {
    *this = ::std::move(from);
  }

  inline person& operator=(const person& from) {
    CopyFrom(from);
    return *this;
  }
  inline person& operator=(person&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const person& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const person* internal_default_instance() {
    return reinterpret_cast<const person*>(
               &_person_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(person* other);
  friend void swap(person& a, person& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline person* New() const final {
    return CreateMaybeMessage<person>(nullptr);
  }

  person* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<person>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const person& from);
  void MergeFrom(const person& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  #if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  #else
  bool MergePartialFromCodedStream(
      ::PROTOBUF_NAMESPACE_ID::io::CodedInputStream* input) final;
  #endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  void SerializeWithCachedSizes(
      ::PROTOBUF_NAMESPACE_ID::io::CodedOutputStream* output) const final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(person* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "person";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_person_2eproto);
    return ::descriptor_table_person_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // bytes name = 1;
  void clear_name();
  static const int kNameFieldNumber = 1;
  const std::string& name() const;
  void set_name(const std::string& value);
  void set_name(std::string&& value);
  void set_name(const char* value);
  void set_name(const void* value, size_t size);
  std::string* mutable_name();
  std::string* release_name();
  void set_allocated_name(std::string* name);

  // string email = 2;
  void clear_email();
  static const int kEmailFieldNumber = 2;
  const std::string& email() const;
  void set_email(const std::string& value);
  void set_email(std::string&& value);
  void set_email(const char* value);
  void set_email(const char* value, size_t size);
  std::string* mutable_email();
  std::string* release_email();
  void set_allocated_email(std::string* email);

  // int32 age = 3;
  void clear_age();
  static const int kAgeFieldNumber = 3;
  ::PROTOBUF_NAMESPACE_ID::int32 age() const;
  void set_age(::PROTOBUF_NAMESPACE_ID::int32 value);

  // @@protoc_insertion_point(class_scope:person)
 private:
  class HasBitSetters;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr email_;
  ::PROTOBUF_NAMESPACE_ID::int32 age_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_person_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// person

// bytes name = 1;
inline void person::clear_name() {
  name_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& person::name() const {
  // @@protoc_insertion_point(field_get:person.name)
  return name_.GetNoArena();
}
inline void person::set_name(const std::string& value) {
  
  name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:person.name)
}
inline void person::set_name(std::string&& value) {
  
  name_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:person.name)
}
inline void person::set_name(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:person.name)
}
inline void person::set_name(const void* value, size_t size) {
  
  name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:person.name)
}
inline std::string* person::mutable_name() {
  
  // @@protoc_insertion_point(field_mutable:person.name)
  return name_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* person::release_name() {
  // @@protoc_insertion_point(field_release:person.name)
  
  return name_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void person::set_allocated_name(std::string* name) {
  if (name != nullptr) {
    
  } else {
    
  }
  name_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), name);
  // @@protoc_insertion_point(field_set_allocated:person.name)
}

// string email = 2;
inline void person::clear_email() {
  email_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& person::email() const {
  // @@protoc_insertion_point(field_get:person.email)
  return email_.GetNoArena();
}
inline void person::set_email(const std::string& value) {
  
  email_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:person.email)
}
inline void person::set_email(std::string&& value) {
  
  email_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:person.email)
}
inline void person::set_email(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  email_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:person.email)
}
inline void person::set_email(const char* value, size_t size) {
  
  email_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:person.email)
}
inline std::string* person::mutable_email() {
  
  // @@protoc_insertion_point(field_mutable:person.email)
  return email_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* person::release_email() {
  // @@protoc_insertion_point(field_release:person.email)
  
  return email_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void person::set_allocated_email(std::string* email) {
  if (email != nullptr) {
    
  } else {
    
  }
  email_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), email);
  // @@protoc_insertion_point(field_set_allocated:person.email)
}

// int32 age = 3;
inline void person::clear_age() {
  age_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 person::age() const {
  // @@protoc_insertion_point(field_get:person.age)
  return age_;
}
inline void person::set_age(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  age_ = value;
  // @@protoc_insertion_point(field_set:person.age)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_person_2eproto
