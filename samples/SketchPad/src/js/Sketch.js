/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit - SketchPad
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 02/05/2011.
 */


// include additional JavaScript sources & libraries via the builtin require(..) function
require("FieldKit.js"); 


var Sketch = function(width, height)
{    
    // This creates a new 2d graphics drawing context.
    // Its API is kept similar to Processing.org
    // for a complete list of supported methods look for Graphics2DWrap::initialize() in
    // FieldKit.cpp/src/fieldkit/script/module/Graphics2D.cpp
	var g = new Graphics2D();
    
    var mouse = new fk.Vec2();
    var frame = 0;
    
	this.update = function()
	{
        frame++;
	};
	
	this.draw = function()
	{		
		g.background(0.99);
        
        g.stroke(1,0,0);
        g.pointSize(15);
        g.point(mouse.x, mouse.y);
        
        g.fill(0);
        g.text("SketchPad "+ Math.round(GetAverageFps()) +"fps", 15, height - 25);
	};
	
    this.mouseDrag = function(event)
    {
        mouse.set2(event.x, event.y);
    };
    
	this.keyDown = function(event) 
    {
        switch(event.char) {
            case ' ':
                break;
                
            case 'r':
                // this calls the custom SaveFrame function we've bound previously via the Utilities module
                SaveFrame("Desktop/SketchPad_"+ frame +".png");
                break;
        }
    };
};