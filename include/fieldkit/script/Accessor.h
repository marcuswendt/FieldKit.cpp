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
		
	class Accessor {};

	//! Maps a C++ class property to JS
	class AccessorProperty : public Accessor {
	public:
		virtual void set(Local<String> property, Local<Value> value, AccessorInfo const& info) = 0;
		virtual Handle<Value> get(Local<String> property, AccessorInfo const& info) = 0;
	};

	//! JS callback
	void Accessor_SetProperty(Local<String> property, Local<Value> value, const AccessorInfo& info);

	//! JS callback
	Handle<Value> Accessor_GetProperty(Local<String> property, const AccessorInfo& info);

} }; // namespace fieldkit::script

