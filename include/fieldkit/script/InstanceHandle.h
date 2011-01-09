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
		
	template <class T>
	class InstanceHandle {
	public:
		InstanceHandle(bool toBeCreated)
		: t_(NULL), toBeDestroy_(toBeCreated),toBeCreated_(toBeCreated) {};
		
		explicit InstanceHandle(T* t) 
		: t_(t), toBeDestroy_(false), toBeCreated_(false) {}
		
		~InstanceHandle() { if (t_ && toBeDestroy_) delete t_; }
		
		inline void set(T*t, bool toBeDestroy);
		
		/**
		 * Returns true if the handle is empty.
		 */
		inline bool IsEmpty();
		
		T* operator->();
		
		T* operator*();
		
		/**
		 * Checks whether two handles are the same.
		 * Returns true if both are empty, or if the objects
		 * to which they refer are identical.
		 * The handles' references are not checked.
		 */
		template <class S> bool operator==(v8::Handle<S> that);
		
		/**
		 * Checks whether two handles are different.
		 * Returns true if only one of the handles is empty, or if
		 * the objects to which they refer are different.
		 * The handles' references are not checked.
		 */
		template <class S> bool operator!=(v8::Handle<S> that) 
		{
			return !operator==(that);
		}
		
		static InstanceHandle<T>* Cast(void* that) 
		{
			return static_cast<InstanceHandle<T>*>(that);
		}
		
	private:
		T* t_;
		bool toBeDestroy_;
		bool toBeCreated_;
	};

	template <class T>
	T* InstanceHandle<T>::operator->()
	{
		if (!t_ && toBeCreated_) {
			LOG_INFO("InstanceHandle<T>::operator toBeCreated");
			t_ = Type::New((T*)NULL);
			toBeCreated_ = false;
		}
		
		return t_;
	}

	template <class T>
	void InstanceHandle<T>::set(T*t, bool toBeDestroy)
	{ 
		if (t_ && toBeDestroy_) 
			delete t_; 
		toBeDestroy = toBeDestroy; 
		t_ = t;
	}

	template <class T>
	template <class S>
	bool InstanceHandle<T>::operator==(v8::Handle<S> that)
	{
		if (!t_ && toBeCreated_)
			return false;
		
		void** a = reinterpret_cast<void**>(**this);
		void** b = reinterpret_cast<void**>(*that);
		if (a == 0) return b == 0;
		if (b == 0) return false;
		return *a == *b;
	}


	template <class T>
	bool InstanceHandle<T>::IsEmpty() 
	{ 
		return t_ == 0 && !toBeCreated_; 
	}

	template <class T>
	T* InstanceHandle<T>::operator*()
	{
		if (!t_ && toBeCreated_) {
			t_ = Type::New((T*)NULL);
			toBeCreated_ = false;
		}
		
		return t_;
	}

} }; // namespace fieldkit::script