# NLang
NLang is a compiled, static-typed, imperative programming language. The syntax is not final and will change.

## Syntax

### Constants and Variable
    // constant (type infer)
    const const1 = 1
    // typed constant
    const const2 = 1 as i32
    // variable (type infer)
    let var1 = 1
    // typed variable
    let var2 = 1 as i32

### Flow Control
`<init>` is a variable definition, assignment or expression statement, `<cond>` is an expression that returns a boolean, `<incr>` is a assignment or expression statement
	
	// if statement
	
	// if <init>; <cond> {}
	if let ch = get_current_char(); ch != -1 {
		// body...
	}
	// if <cond> {}
	if current_mode == DEBUG {
		// body...
	}

	// for statement
	
	// for <init>; <cond>; <incr> {}
	for let i = 0; i < 10; i++ { 
		// body...
	}
	// for <init>; <cond> {}
	for let running = true; running {
		// body...
	}
	// for <cond> {}
	for work_is_done() {
		// body...
	}
	// for {}
	for {
		// body...
	}
    
    // switch <expr> { case (<expr>, )* <expr>: ; default: ; }
    switch value {
    case 0:
        println("got 0")
    case 1, 2:
        println("got 1 or 2")
    default:
        println("got something else")
    }
    // switch { case (<expr>, )* <cond>: ; default: ; }
    switch {
    case get_number() < 10, get_float() < 10:
        println("cond 1")
    case get_string() == "":
        println("cond 2")
    default:
        println("no cond")
    }
	
	
### Functions
    // function (return type inferred)
    fn func1(param i32) {
        return param1 * 2
    }
    // function
    fn func2(a u32, b u32) u32 {
        return a + b;
    }
    // lambda function
    let func3 = fn() String {
        return "Awesome"
    }
    // lambda expression
    let func4 = fn(x i32) => x**2

### Types
    // struct
    type vector2 {
        pub x f32
        pub y f32
    }
    fn vector2.length(v vector2) {
        return sqrt(v.x * v.x + v.y * v.y)
    }
    fn vector2.add(v vector2, o vector2) vector2 {
        return vector2{v.x + o.x, v.y + o.y}
    }
    let v1 = vector2{1, 1}
    let v2 = vector2{2, 2}
    let v3 = v1.add(v2)
    
    // classes
    type complex {
    	real f32
    	imag f32
    }
    // constructor
    fn complex.complex(real, imag) {
        return complex{real, imag}
    }
    let c1 = complex{10, 0} // error: type contains private fields
    let c2 = complex(10, 0) // ok

    type destructable {
        val i32
    }
    fn destructable.destructable(val i32) {
        println("constructor called")
        return destructable{val}
    }
    fn destructable.~destructable() {
        println("destructor called")
    }
    
    // new scope
    {
        let d = destructable(10) // prints "constructor called"
        // prints "destructor called"
    }
    
    
### Arrays and Views
    // arrays (stack or static memory)
    let arr1 = [1, 2, 3] // typeof(arr1) is [i32, 3], arr1 is [1, 2, 3] 
    let arr2 = [0; 4] // typeof(arr2) is [i32, 4], arr2 is [0, 0, 0, 0]

    // views (pointer + size)
    let view1 = arr1[:] // typeof(view1) is [i32], view1 is [1, 2, 3]
    let view2 = arr1[1:-1] // view2 is [2]

### References and Pointers

    let val1 = 1 // typeof(val1) is i32
    let ref1 = &val1 // typeof(ref1) is &i32
    let ptr1 = &val1 as *i32 // typeof(ptr1) is *i32
    
    // lvalue reference arguments
    fn increment1(num &i32) {
        num += 1
    }
    increment1(val1) // val1 == 2
    increment1(ref1) // val1 == 3
    increment1(*ptr1) // val1 == 4
    increment1(1) // error, can't convert rvalue to lvalue
    
    // rvalue reference arguments
    fn increment2(num &&i32) {
        num += 1
        return num 
    }
    increment2(val1) // val1 == 5, returns 5
    increment2(1) // returns 2
    increment2(0 + val1) // val1 == 5, returns 6

### Dynamic memory allocation
Usually not needed, for library writers only.

    let raw1 = malloc(typeof(u32).size * 32) // typeof(raw1) is *i32
    let raw2 = raw2 as *const u32 // constant pointer
    
    // pointer aritmetics
    let raw3 = raw1 + 8
    *raw3 = 10
    
    // view form raw pointer
    let view3 = raw1[:32]
    
    // must call free
    free(raw1)

### Standard smart pointers
What should be used instead of malloc. Smart pointers contents are automatically free when they go out of scope.

    let ptr1 = Ptr[u32](10) // typeof(ptr1) is Ptr[u32]
    let raw1 = ptr1.raw() // typeof(ptrraw1) is *u32
    
    // Ptr[T].operator*() returns &T
    let val1 = *ptr1 // typeof(val1) is u32
    let ref1 = &(*ptr1) // typeof(ref1) is &u32
    
	let ptr2 = ShPtr[u32](0)
	// ptr2.refs() is 1
	{
		let ptrs = [ptr2, ptr2, ptr2] // typeof(ptrs) is [ShPtr[u32]; 3]
		// ptr2.refs() is 4
		for ptr in ptrs {
			*ptr += 1
		}
	}
	// ptr2.refs() is 1
	// *ptr2 is 3
