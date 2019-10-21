//-----------------------------------------------------------------------------
// Variant.h
// Copyright (c) 2008.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _VARIANT_H_
#define _VARIANT_H_
#include <typeinfo>

namespace dbix{

/**
 * \brief   A class to hold a generic value. 
 * \details The Variant class is type safe and can hold values of almost any type.
 *          The requirements for the value types are:
 *          - Has a copy constructor. 
 *          - Is assignable.
 *          These requirements excludes arrays for example.
 */
class Variant
{
public:

   /**
    * \brief  Constructs an empty variant.
    */
   Variant() : m_value(NULL) {}

   /**
    * \brief  Copy constructor.
    * \param  variant Variant to copy.
    */
   Variant(const Variant& variant)
   {
      m_value = variant.m_value ? variant.m_value->Clone() : NULL;
   }

   /**
    * \brief   Constructs a variant from a variable of (almost) any type.
    * \details Arrays can't be used in this variant implementation. 
    *          std::vector, std::string or similar should be used instead.
    * \tparam  T Value type.
    * \param   value Value to put in the variant.
    */
   template<typename T>
   Variant(const T& value)
   {
      m_value = new ValueWrapperImpl<T>(value);
   }

   /**
    * \brief  Copy operator.
    * \param  variant Variant to copy.
    * \return A reference to the target variant.
    */
   Variant& operator=(const Variant& variant)
   {
      delete m_value;
      m_value = variant.m_value ? variant.m_value->Clone() : NULL;
      return *this;
   }

   /**
    * \brief  Generic copy operator. Assigns a value to the variant.
    * \tparam T Value type.
    * \param  value New value for the variant.
    * \return A reference to the target variant.
    */
   template<typename T>
   Variant& operator=(const T& value)
   {
      delete m_value;
      m_value = new ValueWrapperImpl<T>(value);
      return *this;
   }

   /**
    * \brief   Checks if a variant has a value.
    * \return  true if the variant does have a value, otherwise false.
    */
   bool HasValue() const  
   {
      return m_value != NULL;
   }

   /**
    * \brief  Copies the variants value to a variable.
    * \param  value Reference to variable that will receive the value from the variant.
    * \return Returns true if the value could be copied or false if incorrect type or the variant is empty.
    */
   template<typename T>
   bool CopyTo(T& value) const
   {
      const T* pointer = GetPointer<T>();
      if(pointer)
         value = *pointer;
      return pointer != NULL;
   }

   /**
    * \brief   Gets a pointer to the variants value. 
    * \details It's usually better to use variant_cast instead of this method, since it can't get in trouble with things going out of scope.
    * \tparam  T Value type.
    * \return  Pointer to the variants value, or NULL if variant type is not the same as specified value type.
    */
   template<typename T>
   const T* GetPointer() const
   {
      if(GetType() == typeid(T))
         return &static_cast< ValueWrapperImpl<T>* >(m_value)->m_value;
      return NULL;
   }

   virtual ~Variant() 
   {
      delete m_value;
   }
   
   /**
    * \brief  Returns the type info for the variants value.
    * \return Type info describing the variants value or, if empty, the type info for void.
    */
   const std::type_info& GetType() const
   {
      return m_value ? m_value->GetType() : typeid(void);
   }

private:
   
   class ValueWrapper
   {
   public:
      virtual ~ValueWrapper() {}
      virtual const std::type_info& GetType() const = 0;
      virtual ValueWrapper* Clone() const = 0;
   private:
      ValueWrapper& operator=(const ValueWrapper&);
   };

   template<typename T>
   class ValueWrapperImpl : public ValueWrapper
   {
   public:
      ValueWrapperImpl(const T& value) : m_value(value) {}
      const std::type_info& GetType() const
      {
         return typeid(T);
      }
      ValueWrapper* Clone() const
      {
         return new ValueWrapperImpl(m_value);
      }
      const T m_value;
   };

   ValueWrapper* m_value;
};

/**
 * \brief     Casts a variant to a specific type. 
 * \tparam    T Value type.
 * \param     variant Variant to cast.
 * \exception std::bad_cast Thrown if the type of the variant does not match the template type.
 * \return    The value of the variant.
 */
template<typename T>
T variant_cast(const Variant& variant)
{
   const T* pointer = variant.GetPointer<T>();
   if(!pointer)
      throw std::bad_cast();
   return *pointer;
}

/**
* \brief   Casts a variant to a specific type.
* \details In contrast to variant_cast, this cast does not throw an exception if the types don't match. 
*          Instead a default is returned.
* \tparam  T Value type.
* \param   variant Variant to cast.
* \param   defaultValue Value to return if types does not match, or if the variant is empty.
* \return  The value of the variant, or the default value.
*/
template<typename T>
T variant_cast_default(const Variant& variant, const T& defaultValue = T())
{
   try
   {
      return variant_cast<T>(variant);
   }
   catch (std::bad_cast&)
   {
      return defaultValue;
   }
}

}; // namespace dbix
#endif
