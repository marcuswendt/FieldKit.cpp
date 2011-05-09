/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 09/05/2011.
 */

#include "cinder/ImageIo.h"

namespace fieldkit { namespace gl {
    
	class CompressorWorker;
    
    /**
     * 
     */
    class ImageCompressor {
    public:
        ImageCompressor() {}
        ~ImageCompressor();
        void Compress(std::string file, ci::Surface image);
	private:
		std::vector<CompressorWorker*> workers;
    };
    
} }