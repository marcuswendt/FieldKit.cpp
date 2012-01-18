/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit.js
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2012, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 03/04/2011.
 */

var fk = fk || {};
//  FieldKit.js — Copyright 2011 FIELD. All rights reserved.
/*
	Class-like inheritance
	this code was adapted from John Resig's Simple JavaScript inheritance article
	http://ejohn.org/blog/simple-javascript-inheritance
*/
 (function()
 {
    var initializing = false,
    fnTest = /xyz/.test(function() {
        xyz;
    }) ? /\b_super\b/: /.*/;
    // The base Class implementation (does nothing)
    fk.Class = function() {};

    // Create a new Class that inherits from this class
    fk.Class.extend = function(prop) {
        var _super = this.prototype;

        // Instantiate a base class (but only create the instance,
        // don't run the init constructor)
        initializing = true;
        var prototype = new this();
        initializing = false;

        // Copy the properties over onto the new prototype
        for (var name in prop) {
            // Check if we're overwriting an existing function
            prototype[name] = typeof prop[name] == "function" &&
            typeof _super[name] == "function" && fnTest.test(prop[name]) ?
            (function(name, fn) {
                return function() {
                    var tmp = this._super;

                    // Add a new ._super() method that is the same method
                    // but on the super-class
                    this._super = _super[name];

                    // The method only need to be bound temporarily, so we
                    // remove it when we're done executing
                    var ret = fn.apply(this, arguments);
                    this._super = tmp;

                    return ret;
                };
            })(name, prop[name]) :
            prop[name];
        }

        // The dummy class constructor
        function Class() {
            // All construction is actually done in the init method
            if (!initializing && this.init)
            this.init.apply(this, arguments);
        }

        // Populate our constructed prototype object
        Class.prototype = prototype;

        // Enforce the constructor to be what we expect
        Class.constructor = Class;

        // And make this class extendable
        Class.extend = arguments.callee;

        return Class;
    };
})();
//  FieldKit.js — Copyright 2011 FIELD. All rights reserved.

(function() {

	fk.math = fk.math || {};
	var ns = fk.math;

	// Constants
	ns.PI = Math.PI;
	ns.TWO_PI = Math.PI * 2.0;
	ns.HALF_PI = Math.PI * 0.5;
	ns.EPSILON = 0.001;
	ns.DEG_TO_RAD = Math.PI / 180;
	ns.RAD_TO_DEG = 180 / Math.PI;
	
	// Utilities
	ns.min = function(a, b) {
	  if (a < b) return a;
	  return b;
	};

	ns.max = function(a, b) {
	  if (a > b) return a;
	  return b;
	};

	// Keep the value |v| in the range vmin .. vmax.  This matches GLSL clamp().
	ns.clamp = function(v, vmin, vmax) {
	  return ns.min(vmax, ns.max(vmin, v));
	};
	
	ns.lerp = function(current, target, delta)
	{
		return current * (1 - delta) + target * delta;
	};

	ns.round = function(value, dec) 
	{
		var exp = Math.pow(10,dec);
		return Math.floor(value * exp) / exp;
	};
	
}()); 
//  FieldKit.js — Copyright 2011 FIELD. All rights reserved.

fk.Vec2 = function() {
	function Vec2(x,y) { this.x = x; this.y = y; }

	Vec2.prototype.set2 = function(x,y) { this.x = x; this.y = y; return this; };
	Vec2.prototype.set = function(v) { this.x = v.x; this.y = v.y; return this; };
	Vec2.prototype.zero = function() { this.x = 0; this.y = 0; return this; };

	Vec2.prototype.add = function(v) { this.x += v.x; this.y += v.y; return this; };
	Vec2.prototype.add_ = function(v) { return new Vec2(this.x + v.x, this.y + v.y); };
	
	Vec2.prototype.sub = function(v) { this.x -= v.x; this.y -= v.y; return this; };
	Vec2.prototype.sub_ = function(v) { return new Vec2(this.x - v.x, this.y - v.y); };
    
	Vec2.prototype.mul = function(v) { this.x *= v.x; this.y *= v.y; return this; };
	Vec2.prototype.mul_ = function(v) { return new Vec2(this.x * v.x, this.y * v.y); };
	
	Vec2.prototype.div = function(v) { this.x /= v.x; this.y /= v.y; return this; };
	Vec2.prototype.div_ = function(v) { return new Vec2(this.x / v.x, this.y / v.y); };

	Vec2.prototype.scale = function(value) { this.x *= value; this.y *= value; return this; };
	Vec2.prototype.scale_ = function(value) { return new Vec2(this.x * value, this.y * value); };

	Vec2.prototype.length = function() { return Math.sqrt(this.x * this.x + this.y * this.y); };
	Vec2.prototype.lengthSquared = function() { return this.x * this.x + this.y * this.y; };

	Vec2.prototype.normalize = function() 
	{
		var l = this.length();
		if(l != 0) {
			this.x /= l;
			this.y /= l;
		}
		return this;
	};
	
	Vec2.prototype.normalizeTo = function(length)
	{
		var magnitude = Math.sqrt(this.x * this.x + this.y * this.y);
		if(magnitude > 0) {
			magnitude = length / magnitude;
			this.x *= magnitude;
			this.y *= magnitude;
		}
		return this;
	};

	Vec2.prototype.distance = function(v) { return Math.sqrt(this.distanceSquared2(v.x, v.y)); };
	Vec2.prototype.distanceSquared = function(v) { return this.distanceSquared2(v.x, v.y); };	
	Vec2.prototype.distanceSquared2 = function(x, y) 
	{ 
		var dx = this.x - x;
		var dy = this.y - y;
		return dx * dx + dy * dy;
	};


	Vec2.prototype.dot = function(v) { return this.x * v.x + this.y * v.y; };
	
	Vec2.prototype.rotate = function(angle)
	{
		var sina = Math.sin(angle);
		var cosa = Math.cos(angle);
		var rx = this.x * cosa - this.y * sina;
		var ry = this.x * sina + this.y * cosa;
		this.x = rx;
		this.y = ry;	
	};

	Vec2.prototype.jitter = function(amount) 
	{ 
		this.x += Random.float(-1,1) * amount;
		this.y += Random.float(-1,1) * amount;
		return this;
	};
	
	Vec2.prototype.jitter_ = function(amount) { return (new Vec2(this.x, this.y)).jitter(amount); };

	Vec2.prototype.lerp = function(target, delta)
	{
		this.x = this.x * (1 - delta) + target.x * delta;
		this.y = this.y * (1 - delta) + target.y * delta;
		return this;
	};
	
	Vec2.prototype.lerp_ = function(target, delta)
	{
		return (new Vec2(this.x, this.y)).lerp(target, delta);
	};
	
	Vec2.prototype.equals = function(v)	
	{
		return this.x == v.x && this.y == v.y;
	};
	
	Vec2.prototype.clone = function()
	{
		return new Vec2(this.x, this.y);
	};

	Vec2.prototype.toString = function() { return "Vec2["+ this.x +","+ this.y +"]"; };
	
	return Vec2;
}();

fk.Vec3 = function() 
{
	function Vec3(x,y,z) { this.x = x; this.y = y; this.z = z; }

	Vec3.prototype.set = function(v) { this.x=v.x; this.y=v.y; this.z=v.z; return this; };
	Vec3.prototype.set3 = function(x,y,z) { this.x=x; this.y=y; this.z=z; return this; };
	Vec3.prototype.zero = function() { this.x=0; this.y=0; this.z=0; return this; };

	Vec3.prototype.add = function(v) { this.x += v.x; this.y += v.y; this.z += v.z; return this; };
	Vec3.prototype.add_ = function(v) { return new Vec3(this.x + v.x, this.y + v.y, this.z + v.z); };
	Vec3.prototype.add3 = function(x,y,z) { this.x += x; this.y += y; this.z += z; return this; };
		
	Vec3.prototype.sub = function(v) { this.x -= v.x; this.y -= v.y; this.z -= v.z; return this; };
	Vec3.prototype.sub_ = function(v) { return new Vec3(this.x - v.x, this.y - v.y, this.z - v.z); };
	Vec3.prototype.sub3 = function(x,y,z) { this.x -= x; this.y -= y; this.z -= z; return this; };
	    
	Vec3.prototype.mul = function(v) { this.x *= v.x; this.y *= v.y; this.z *= v.z; return this; };
	Vec3.prototype.mul_ = function(v) { return new Vec3(this.x * v.x, this.y * v.y, this.z * v.z); };
	Vec3.prototype.mul3 = function(x,y,z) { this.x *= x; this.y *= y; this.z *= z; return this; };
		
	Vec3.prototype.div = function(v) { this.x /= v.x; this.y /= v.y; this.z /= v.z; return this; };
	Vec3.prototype.div_ = function(v) { return new Vec3(this.x / v.x, this.y / v.y, this.z / v.z); };
	Vec3.prototype.div3 = function(x,y,z) { this.x /= x; this.y /= y; this.z /= z; return this; };
	
	Vec3.prototype.scale = function(value) { this.x *= value; this.y *= value; this.z *= value; return this; };
	Vec3.prototype.scale_ = function(value) { return new Vec3(this.x * value, this.y * value, this.z * value); };

	Vec3.prototype.length = function() { return Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z); };
	Vec3.prototype.lengthSquared = function() { return this.x * this.x + this.y * this.y + this.z * this.z; };

	Vec3.prototype.normalize = function() 
	{
		var l = this.length();
		if(l != 0) {
			this.x /= l;
			this.y /= l;
			this.z /= l;
		}
		return this;
	};
	
	Vec3.prototype.normalizeTo = function(length)
	{
		var magnitude = Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z);
		if(magnitude > 0) {
			magnitude = length / magnitude;
			this.x *= magnitude;
			this.y *= magnitude;
			this.z *= magnitude;
		}
		return this;
	};

	Vec3.prototype.distance = function(v) { return Math.sqrt(this.distanceSquared3(v.x, v.y, v.z)); };
	Vec3.prototype.distanceSquared = function(v) { return this.distanceSquared2(v.x, v.y, v.z); };	
	Vec3.prototype.distanceSquared3 = function(x, y, z) 
	{ 
		var dx = this.x - x;
		var dy = this.y - y;
		var dz = this.z - z;
		return dx * dx + dy * dy + dz * dz;
	};


	Vec3.prototype.dot = function(v) { return this.x * v.x + this.y * v.y + this.z * v.z; };
	
	Vec3.prototype.jitter = function(amount) 
	{ 
		this.x += Random.float(-1,1) * amount;
		this.y += Random.float(-1,1) * amount;
		this.z += Random.float(-1,1) * amount;
		return this;
	};
	
	Vec3.prototype.jitter_ = function(amount) { return (new Vec3(this.x, this.y, this.z)).jitter(amount); };

	Vec3.prototype.lerp = function(target, delta)
	{
		this.x = this.x * (1 - delta) + target.x * delta;
		this.y = this.y * (1 - delta) + target.y * delta;
		this.z = this.z * (1 - delta) + target.z * delta;
		return this;
	};
	
	Vec3.prototype.lerp_ = function(target, delta)
	{
		return (new Vec3(this.x, this.y, this.z)).lerp(target, delta);
	};
	
	Vec3.prototype.equals = function(v)	
	{
		return this.x == v.x && this.y == v.y && this.z == v.z;
	};
	
	Vec3.prototype.clone = function()
	{
		return new Vec3(this.x, this.y, this.z);
	};

	Vec3.prototype.toString = function() { return "Vec3["+ this.x +","+ this.y +","+ this.z +"]"; };
	
	return Vec3;
}();

//  FieldKit.js — Copyright 2011 FIELD. All rights reserved.

/**
 * A simple line from multiple segments, can be sampled at any point.
 * NOTE this curve does not always run through all given points.
 * Work with any object type as long as it has x and y number properties.
 */
fk.Polyline = function()
{
	var points = [];

	this.add = function(point) { points.push(point); };
	this.points = function() { return points; };
    this.size = function() { return points.length; };
    this.clear = function() { points = []; };

    this.point = function(time)
    {
        // first point
        if(time <= 0) {
            return points[0];

        // last point
        } else if (time >= 1) {
            return points[points.length - 1];

        // in between
        } else {
            var median = time * (points.length - 1);
            var prev = points[Math.floor(median)];
            var next = points[Math.ceil(median)];

            return {
                x: prev.x + (next.x - prev.x) * 0.5,
                y: prev.y + (next.y - prev.y) * 0.5
            };
        }
    };
};
//  FieldKit.js — Copyright 2011 FIELD. All rights reserved.

/** 
* A Catmull-Rom spline (which is a special form of the cubic hermite curve) implementation, 
* generates a smooth curve/interpolation from a number of Vec2 points.
*/
fk.Spline = function()
{
	var points = [];
	var needsUpdate = false;
	var first, second, beforeLast, last;
	
	this.add = function(point)
	{
		points.push(point);
		needsUpdate = true;
	};
	
	this.points = function() { return points; };
    this.size = function() { return points.length; };
    this.clear = function() { points = []; };

	var update = function()
	{
		if(points.length < 4) return;
		first = points[0];
		second = points[1];
		beforeLast = points[points.length-2];
		last = points[points.length-1];
		needsUpdate = false;
	};

    this.point = function(time)
    {			
		if(points.length < 4) return;

        // first point
        if (time <= 0) {
            return points[0];

        // last point
        } else if (time >= 1) {
            return points[points.length - 1];

        // in between
        } else {
			if(needsUpdate)
				update();
	
            var size = points.length;
            var partPercentage = 1.0 / (size - 1);
            var timeBetween = time / partPercentage;

            var i = Math.floor(timeBetween);
            var normalizedTime = timeBetween - i;

            var t = normalizedTime * 0.5;
            var t2 = t * normalizedTime;
            var t3 = t2 * normalizedTime;

			var tmp1;
            if(--i == -1) {
				// var tmp0 = second.sub_(first).normalizeTo(Number.MIN_VALUE);
				// tmp1 = first.sub_(tmp0);
				tmp1 = first.clone();
            } else {
				tmp1 = points[i].clone();
            }

            var tmp2 = points[++i].clone();
            var tmp3 = points[++i].clone();

			var tmp4;
            if(++i == size) {
				// var tmp0 = beforeLast.sub_(last).normalizeTo(Number.MIN_VALUE);
				// tmp4 = last.sub_(tmp0);
				tmp4 = last.clone();
            } else {
				tmp4 = points[i].clone();
            }

            // calculate point
			tmp1.scale(-t3 + 2 * t2 - t);
			var result = tmp1;

			tmp2.scale(3 * t3 - 5 * t2 + 1);
			result.add(tmp2);

			tmp3.scale(-3 * t3 + 4 * t2 + t);
			result.add(tmp3);

			tmp4.scale(t3 - t2);
			result.add(tmp4);
			
            return result;
        }
    };
};
