////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2016 by EMC Corporation, All Rights Reserved
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is EMC Corporation
///
/// @author Andrey Abramov
////////////////////////////////////////////////////////////////////////////////

#include "analysis/token_attributes.hpp"
#include "index/iterators.hpp"
#include "index/field_meta.hpp"
#include "formats/empty_term_reader.hpp"
#include "search/cost.hpp"
#include "utils/type_limits.hpp"
#include "utils/singleton.hpp"

namespace {

//////////////////////////////////////////////////////////////////////////////
/// @class empty_doc_iterator
/// @brief represents an iterator with no documents 
//////////////////////////////////////////////////////////////////////////////
struct empty_doc_iterator final : irs::doc_iterator {
  empty_doc_iterator() noexcept
    : cost(0),
      doc{irs::doc_limits::eof()} {
  }

  virtual irs::doc_id_t value() const override {
    return irs::doc_limits::eof();
  }
  virtual bool next() override { return false; }
  virtual irs::doc_id_t seek(irs::doc_id_t) override {
    return irs::doc_limits::eof();
  }
  virtual irs::attribute* get_mutable(irs::type_info::type_id type) noexcept override {
    if (irs::type<irs::document>::id() == type) {
      return &doc;
    }

    return irs::type<irs::cost>::id() == type
      ? &cost
      : nullptr;
  }

  irs::cost cost;
  irs::document doc{irs::doc_limits::eof()};
}; // empty_doc_iterator

empty_doc_iterator EMPTY_DOC_ITERATOR;

//////////////////////////////////////////////////////////////////////////////
/// @class empty_term_iterator
/// @brief represents an iterator without terms
//////////////////////////////////////////////////////////////////////////////
struct empty_term_iterator : irs::term_iterator {
  virtual const irs::bytes_ref& value() const noexcept override final {
    return irs::bytes_ref::NIL;
  }
  virtual irs::doc_iterator::ptr postings(irs::IndexFeatures) const noexcept override final {
    return irs::doc_iterator::empty();
  }
  virtual void read() noexcept override final { }
  virtual bool next() noexcept override final { return false; }
  virtual irs::attribute* get_mutable(irs::type_info::type_id) noexcept override final {
    return nullptr;
  }
}; // empty_term_iterator

empty_term_iterator EMPTY_TERM_ITERATOR;

//////////////////////////////////////////////////////////////////////////////
/// @class empty_seek_term_iterator
/// @brief represents an iterator without terms
//////////////////////////////////////////////////////////////////////////////
struct empty_seek_term_iterator final : irs::seek_term_iterator {
  virtual const irs::bytes_ref& value() const noexcept override final {
    return irs::bytes_ref::NIL;
  }
  virtual irs::doc_iterator::ptr postings(irs::IndexFeatures) const noexcept override final {
    return irs::doc_iterator::empty();
  }
  virtual void read() noexcept override final { }
  virtual bool next() noexcept override final { return false; }
  virtual irs::attribute* get_mutable(irs::type_info::type_id) noexcept override final {
    return nullptr;
  }
  virtual irs::SeekResult seek_ge(const irs::bytes_ref&) noexcept override {
    return irs::SeekResult::END;
  }
  virtual bool seek(const irs::bytes_ref&) noexcept override {
    return false;
  }
  virtual bool seek(const irs::bytes_ref&, const irs::seek_cookie&) noexcept override {
    return false;
  }
  virtual irs::seek_cookie::ptr cookie() const noexcept override {
    return nullptr;
  }
}; // empty_seek_term_iterator

empty_seek_term_iterator EMPTY_SEEK_TERM_ITERATOR;

//////////////////////////////////////////////////////////////////////////////
/// @brief represents a reader with no terms
//////////////////////////////////////////////////////////////////////////////
const irs::empty_term_reader EMPTY_TERM_READER{0};

//////////////////////////////////////////////////////////////////////////////
/// @class empty_field_iterator
/// @brief represents a reader with no fields
//////////////////////////////////////////////////////////////////////////////
struct empty_field_iterator final : irs::field_iterator {
  virtual const irs::term_reader& value() const override {
    return EMPTY_TERM_READER;
  }

  virtual bool seek(const irs::string_ref&) override {
    return false;
  }

  virtual bool next() override {
    return false;
  }
}; // empty_field_iterator

empty_field_iterator EMPTY_FIELD_ITERATOR;

//////////////////////////////////////////////////////////////////////////////
/// @class empty_column_iterator
/// @brief represents a reader with no columns
//////////////////////////////////////////////////////////////////////////////
struct empty_column_iterator final : irs::column_iterator {
  virtual const irs::column_meta& value() const override {
    static const irs::column_meta EMPTY;
    return EMPTY;
  }

  virtual bool seek(const irs::string_ref&) override {
    return false;
  }

  virtual bool next() override {
    return false;
  }
}; // empty_column_iterator

empty_column_iterator EMPTY_COLUMN_ITERATOR;

} // LOCAL

namespace iresearch {

// ----------------------------------------------------------------------------
// --SECTION--                                                    term_iterator
// ----------------------------------------------------------------------------

term_iterator::ptr term_iterator::empty() {
  return memory::to_managed<irs::term_iterator, false>(&EMPTY_TERM_ITERATOR);
}

// ----------------------------------------------------------------------------
// --SECTION--                                               seek_term_iterator
// ----------------------------------------------------------------------------

seek_term_iterator::ptr seek_term_iterator::empty() {
  return memory::to_managed<irs::seek_term_iterator, false>(&EMPTY_SEEK_TERM_ITERATOR);
}

// ----------------------------------------------------------------------------
// --SECTION--                                                seek_doc_iterator 
// ----------------------------------------------------------------------------

doc_iterator::ptr doc_iterator::empty() {
  return memory::to_managed<doc_iterator, false>(&EMPTY_DOC_ITERATOR);
}

// ----------------------------------------------------------------------------
// --SECTION--                                                   field_iterator 
// ----------------------------------------------------------------------------

field_iterator::ptr field_iterator::empty() {
  return memory::to_managed<irs::field_iterator, false>(&EMPTY_FIELD_ITERATOR);
}

// ----------------------------------------------------------------------------
// --SECTION--                                                  column_iterator 
// ----------------------------------------------------------------------------

column_iterator::ptr column_iterator::empty() {
  return memory::to_managed<irs::column_iterator, false>(&EMPTY_COLUMN_ITERATOR);
}

} // ROOT 
