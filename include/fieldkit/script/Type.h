/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 15/11/2010.
 */

#pragma once

namespace fieldkit { namespace script { namespace Type {
	
	/** Constructor C++ */
	template <class T>
	inline T* New(T*t) { 
		//ASSERT(!t);
		return new T();
	}
	
	/** Cast from V8 to C++ */
	inline bool Cast(bool*, Local<Value> value) { 
		return value->BooleanValue(); 
	}	
	
	inline float Cast(float*, Local<Value> value) { 
		return (float)value->NumberValue(); 
	}

	inline double Cast(double*, Local<Value> value) { 
		return value->NumberValue(); 
	}
		
	inline uint32_t Cast(uint32_t*, Local<Value> value) { 
		return value->Uint32Value(); 
	}	
	
	inline int32_t Cast(int32_t*, Local<Value> value) { 
		return value->Int32Value(); 
	}
	
	inline int64_t Cast(int64_t*, Local<Value> value) { 
		return value->IntegerValue(); 
	}
		
	inline Handle<Value> Cast(Handle<Value> value) { 
		return value;
	}
	
	
	/** Cast V8:Value to primitive value */
	template<class T>
	inline T Cast(T*, Local<Value> value) { 
		return ProxyClass<T>::Instance()->unwrapObject(value);
	}
	
	template<class T>
	inline T Cast(T&, Local<Value> value) { 
		return ProxyClass<T>::Instance()->unwrapObject(value);
	}
	
	/** Cast from C++ to V8 */
	inline Handle<Value> Cast(bool caseParam) { 
		return Boolean::New(caseParam); 
	}
	
	inline Handle<Value> Cast(float caseParam) { 
		return Number::New(caseParam); 
	}
	
	inline Handle<Value> Cast(double caseParam) { 
		return Number::New(caseParam);
	}  
	
	inline Handle<Value> Cast(uint32_t caseParam) { 
		return Uint32::New(caseParam); 
	}	
	
	inline Handle<Value> Cast(int32_t caseParam) { 
		return Int32::New(caseParam);
	}
	
	inline Handle<Value> Cast(char const* caseParam) { 
		return String::New(caseParam);
	}
	
	inline Handle<Value> Cast(std::string const& caseParam) { 
		return String::New(caseParam.c_str());
	}
	
	template<class T>
	inline Handle<Value> Cast(T* t) { 
		return ProxyClass<T>::Instance()->exposeObject(t, false);
	};
	
	template<class T>
	inline Handle<Value> Cast(T& t) { 
		return ProxyClass<T>::Instance()->exposeObject(&t, false);
	};
	
//	/** Function return value Cast from C++ to V8 */
//	inline Handle<Value> FCast(bool caseParam)     { return Cast(caseParam); }	
//	inline Handle<Value> FCast(double caseParam)   { return Cast(caseParam); }  
//	inline Handle<Value> FCast(uint32_t caseParam) { return Cast(caseParam); }	
//	inline Handle<Value> FCast(int32_t  caseParam) { return Cast(caseParam); }
//	
//	template<class T>
//	inline Handle<Value> FCast(T* t) 
//	{ 
//		return ProxyClass<T>::instance()->ExposeObject(t, true);
//	};
//	
//	template<class T>
//	inline Handle<Value> FCast(T& t) 
//	{ 
//		return ProxyClass<T>::instance()->ExposeObject(&t, true);
//	};
	
} } }; // fieldkit::script::Type
