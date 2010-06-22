/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2009, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */
/* created October 27, 2009 */

#ifndef CV_PROPERTY_H
#define CV_PROPERTY_H

using namespace std;

namespace fieldkit { namespace vision {

	class CVProperty
	{
	public:
		CVProperty (string name, float min, float max) { init(name, min, max); };
		~CVProperty();
		
		void init (string name, float min, float max) {
			this->name = name;
			this->min = min;
			this->max = max;
			this->scale = max - min;
			this->value = 0;
		};
		
		void setValue(float v) { 
			v = (v > 1) ? 1 : v;
			v = (v < 0) ? 0 : v;
			value = v;
		};
		
		void setName(string s){name = s;};
		string getName(){return name;};
	
		float getValue() { return value;};
		float get() { return (value*scale) + min;};
		float getMin(){return min;};
		float getMax(){return max;};
		
	private:
		string name;
		float value;
		float scale;
		float min;
		float max;

	};
} } // namespace fieldkit::vision

#endif