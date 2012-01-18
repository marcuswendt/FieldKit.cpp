
// 1. bootstrap the environment 
// define minimal stubs for not yet implemented objects and functions
// to be able to use require.js and cs.js without modifications
importScripts("lib/require.js");

var Sketch = function() {};

var timeOutId = 0;
var setTimeout = function(func, milliseconds) 
{ 
    Sleep(milliseconds);
    func();
    timeOutId++; 
    return timeOutId;
}

var XMLHttpRequest = function() {
	this.open = function(method, url, async)
	{
		this.readyState = 4;
		this.responseText = readFile(url);
	}
	this.onreadystatechange = function(event) {}
	this.send = function() 
	{
		this.onreadystatechange(null);	
	}
};

// 2. run the main coffee-script and let it implement Sketch
require({
  paths: {
    cs: 'lib/cs'
  }
}, ['cs!Sketch'], function(SketchImpl) {
	Sketch = SketchImpl;
	// print("sketch "+ Sketch)
});