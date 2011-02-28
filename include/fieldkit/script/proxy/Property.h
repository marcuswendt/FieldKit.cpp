/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 15/11/2010.
 */

#pragma once

namespace fieldkit { namespace script {

	template<class T, class TProperty>
	class ObjectProperty : public AccessorProperty {
	public:
		typedef TProperty T::*PropertyDir;
		
		ObjectProperty(PropertyDir propertyDir) 
		: propertyDir_(propertyDir) {}
		
		void set(v8::Local<v8::String> property, 
				 v8::Local<v8::Value> value, 
				 const v8::AccessorInfo& info);
		
		v8::Handle<v8::Value> get(v8::Local<v8::String> property, 
								  const v8::AccessorInfo& info);
		
		// TODO remove me?
		//ProxyClass<T>* getTemplateClass() { return templateClass_; };
		
	private:
		PropertyDir propertyDir_;
	};
	
	// Setter implementation for object properties
	template<class T, class TProperty> 
	void ObjectProperty<T,TProperty>::set(v8::Local<v8::String> property, 
										  v8::Local<v8::Value> value, 
										  const v8::AccessorInfo& info)
	{
		T* t = ProxyClass<T>::Instance()->unwrapObject(info.Holder());  
		t->*propertyDir_ = Type::Cast((TProperty*) NULL, value);
	}
	
	// Getter implementation for object properties
	template<class T, class TProperty> 
	v8::Handle<v8::Value> ObjectProperty<T,TProperty>::get(v8::Local<v8::String> property, 
														   const v8::AccessorInfo& info)
	{
		T* t = ProxyClass<T>::Instance()->unwrapObject(info.Holder());
		return Type::Cast((t->*propertyDir_));
	}
	
} }; // namespace fieldkit::script