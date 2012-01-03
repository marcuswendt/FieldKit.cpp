/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 09/05/2011.
 */

#include <boost/thread.hpp>  
#include "cinder/Utilities.h"
#include "fieldkit/gl/ImageCompressor.h"
#include "fieldkit/Logger.h"

namespace fieldkit { namespace gl {
    
	using namespace fieldkit;
    
	struct CompressorJob {
		CompressorJob() : file_("") {}
        
		CompressorJob(std::string file, ci::Surface image)
        : file_(file), image_(image) {}
        
		std::string file_;
		ci::Surface image_;
	};
    
    
	class CompressorWorker {
	public:
		CompressorWorker(int id) : id_(id), quit_(false), isAvailable_(true) 
		{
		}
        
		~CompressorWorker() {
			lock_.lock();
			quit_ = true;
			lock_.unlock();
			thread_.join();
			boost::thread not_a_thread;
			thread_.swap(not_a_thread);
		}
        
		void Start()
		{
			boost::thread thread(boost::thread(&CompressorWorker::ThreadMainThunk, &*this));
			thread_.swap(thread);
		}
        
		bool IsAvailable()
		{
			bool result;
			lock_.lock();
			result = isAvailable_;
			lock_.unlock();
			return result;
		}
        
		void Process(CompressorJob job)
		{
			lock_.lock();
			job_ = job;
			isAvailable_ = false;
			lock_.unlock();
		}
        
		static void ThreadMainThunk(CompressorWorker* worker)
		{
			worker->ThreadMain();
		}
        
		void ThreadMain()
		{
			while(!quit_) {
				lock_.lock();
				if(quit_) {
					lock_.unlock();
					break;
				}
                
				if(isAvailable_) {
					lock_.unlock();
					boost::this_thread::sleep(boost::posix_time::milliseconds(10));
					continue;  // Try again
				}
                
				lock_.unlock();
                
				// do the work
				ci::writeImage(ci::fs::path(job_.file_), job_.image_);
                
				lock_.lock();
				isAvailable_ = true;
				lock_.unlock();
			}
		}
        
	private:
		int id_;
		bool quit_;
		boost::mutex lock_;
		boost::thread thread_;
		bool isAvailable_;
		CompressorJob job_;
	};
    
    
	ImageCompressor::~ImageCompressor()
	{
		std::vector<CompressorWorker*>::iterator it;
		for(it = workers.begin(); it != workers.end(); it++) {
			delete *it;
		}
		workers.clear();
	}
    
    void ImageCompressor::Compress(std::string file, ci::Surface image)
    {
		int kMaxWorkers = 24;
        
		// check all workers repeatedly until one becomes available
		while(true) {
			std::vector<CompressorWorker*>::iterator it;
			for(it = workers.begin(); it != workers.end(); it++) {
				if((*it)->IsAvailable()) {
					LOG_INFO("ImageCompressor::Compress - "<< file);	 
					(*it)->Process( CompressorJob(file, image) );
					return;
				}
			}
            
			// no available worker found && less than the maximum number of workers -> create new thread
			if(workers.size() < kMaxWorkers) {
				LOG_INFO("ImageCompressor::Compress - creating worker ("<< workers.size() <<")");
				CompressorWorker* w = new CompressorWorker(workers.size());
				w->Start();
				workers.push_back(w);
                
                // wait a bit
			} else {	
				boost::this_thread::sleep(boost::posix_time::milliseconds(5));  
			}
		}
    }
    
} }