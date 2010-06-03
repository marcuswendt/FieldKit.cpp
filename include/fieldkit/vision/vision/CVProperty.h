/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2009, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */
/* created October 27, 2009 */

#ifndef CV_PROPERTY_H
#define CV_PROPERTY_H

namespace fk { namespace vision {

	class CVProperty
	{
	public:
		CVProperty (float min, float max) { init(min, max); };
		~CVProperty();
		
		void init (float min, float max) {
			this->min = min;
			this->scale = max - min;
			this->value = 0;
		};
		
		void setValue(float v) { 
			v = (v > 1) ? 1 : v;
			v = (v < 0) ? 0 : v;
			value = v;
		};
		
		float getValue() { return value;};
		
		float get() { return (value*scale) + min;};
		
	private:
		float value;
		float scale;
		float min;
	};
} } // namespace fk::vision

#endif