/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2009, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */
/* created October 27, 2009 */

#ifndef BLOB_H
#define BLOB_H

namespace fieldkit { namespace vision { 

	//
	// represents a single detected element in the scene
	//
	class Blob
	{
		public:
			int id;
			bool isActive;
			bool isAssigned;
			
			CvPoint position;
			CvPoint2D64f position64f;
		
			CvRect bounds;
			CvSeq *contour;
		
			Blob (int id) { 
				this->id = id;
				init();
			};
			~Blob () {};	
			
			
			void init() 
			{
				isActive = false;
				isAssigned = false;
			}
			
			void set(Blob *b) 
			{
				this->position = b->position;
				this->position64f = b->position64f;
				this->bounds = b->bounds;
				this->contour = b->contour;
			}
		
		protected:
	};
} } // namespace fieldkit::vision
#endif