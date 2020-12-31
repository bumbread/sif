# sif
A compiler for a custom language written in C++.

```odin
#include "basic.sif"

proc main() : i32 {
    print_string("-------------------------\n");
    print_string("|   sif language demo   |\n");
    print_string("-------------------------\n");

    basic_stuff();
    arrays();
    slices();
    strings();
    structs();
    enums();
    order_independence();
    // print_string("\n\n---- enums ----\n");

    references();
    operator_overloading();
    procedural_polymorphism();
    structural_polymorphism();
    dynamic_arrays();
    return 0;
}



proc factorial(var n: int) : int {
    if (n == 1) {
        return 1;
    }
    return n * factorial(n-1);
}
proc basic_stuff() {
    print_string("\n\n---- basic_stuff ----\n");

    // declaration syntax:
    // var <variable name>: <type> = <expression>;
    var a: int = 123;

    // type can be omitted if it can be inferred from the right-hand-side expression
    var b1 = 123;
    assert(typeof(b1) == int);
    var b2 = 123.0;
    assert(typeof(b2) == float);

    // since factorial() returns an int, the type can be inferred here too
    var fact = factorial(5);
    assert(fact == 120);
    assert(typeof(fact) == int);

    // number literals are "untyped" and can coerce when needed
    var c: int = 12;
    var d: float = 12;

    // even when nested under binary expressions
    var e: float = 11 / 2;
    assert(e == 5.5);

    // without type inference the "untyped" numbers default to int, as above
    var f = 11 / 2;
    assert(f == 5);

    // if a `.` is present, float will be assumed, as above
    var g = 11.0 / 2;
    assert(g == 5.5);
}



proc arrays_by_value(arr: [4]int) {
    arr[2] = 738;
}
proc arrays() {
    print_string("\n\n---- arrays ----\n");
    var my_array: [4]int;
    my_array[0] = 1;
    my_array[1] = 2;
    my_array[2] = 3;
    my_array[3] = 4;
    print_int(my_array[0]);
    print_int(my_array[1]);
    print_int(my_array[2]);
    print_int(my_array[3]);
    assert(my_array[0] == 1);
    assert(my_array[1] == 2);
    assert(my_array[2] == 3);
    assert(my_array[3] == 4);
    arrays_by_value(my_array);
    print_int(my_array[2]);
    assert(my_array[2] == 3);
}



proc slices() {
    // todo(josh)
    // print_string("\n\n---- slices ----\n");
}



proc strings() {
    print_string("\n\n---- slices ----\n");

    // strings in sif are length-delimited rather than null-terminated.
    // the string type is implemented as:
    // struct String {
    //     var data: rawptr;
    //     var count: int;
    // }
    assert(sizeof(string) == 16);

    // since strings are length-delimited this means you can trivially
    // take a substring without allocating and copying
    var a = "Hello, World!";
    var hello: string;
    hello.data = &a[0];
    hello.count = 5;
    assert(hello == "Hello");
    print_string(hello);
    print_string("\n");

    var world: string;
    world.data = &a[7];
    world.count = 5;
    assert(world == "World");
    print_string(world);
    print_string("\n");
}



struct Foo {
    var a: int;
    var str: string;
    var t: bool;
}
proc structs() {
    print_string("\n\n---- structs ----\n");

    var f: Foo;
    assert(f.a == 0); // everything is zero initialized by default
    f.a = 123;
    f.str = "foozle";
    f.t = 120 == factorial(5);

    // can also use compound literals to initialize
    f = Foo{149, "hellooo", false};
}



proc enums() {
    // todo(josh)
    // print_string("\n\n---- enums ----\n");
}



proc order_independence() {
    print_string("\n\n---- order_independence ----\n");

    // declaration order does not matter. the compiler figures
    // out what depends on what for you. delete all your .h files :)

    var loopy: Loopy;
    assert(sizeof(typeof(loopy.a)) == 8);
    assert(sizeof(typeof(nesty)) == 64);
}
var nesty: [sizeof(typeof(ptr_to_loopy^))]int;
var ptr_to_loopy: ^Loopy;
struct Loopy {
    var a: [sizeof(typeof(ptr_to_loopy))]bool;
}



proc change_by_reference(a: >int, value: int) {
    // since `a` is a reference, it is implicitly a pointer
    // and dereferences/indirections are inserted automatically
    a = value;
}
proc references() {
    print_string("\n\n---- references ----\n");

    var my_int = 123;
    print_int(my_int);
    assert(my_int == 123);

    // reference implicitly take/dereference pointers depending on context
    // and are (currently, will likely change) denoted by a `>` before the type
    var int_reference: >int = my_int; // implicit `&my_int` after `=`
    int_reference = 789;              // implicit `int_reference^` before `=`
    assert(int_reference == 789);
    assert(my_int == 789);
    print_int(int_reference);

    // you can also define procedures that take (and return) references, of course
    assert(my_int == 789);
    change_by_reference(my_int, 456);
    assert(my_int == 456);
    print_int(my_int);

    // the only real reason I implemented references is for overloading []
    // which you can see below in the structural_polymorphism() and
    // dynamic_array() procedures
}



struct Vector3 {
    var x: float;
    var y: float;
    var z: float;
    operator +(a: Vector3, b: Vector3) : Vector3 {
        return Vector3{a.x + b.x, a.y + b.y, a.z + b.z};
    }
    operator -(a: Vector3, b: Vector3) : Vector3 {
        return Vector3{a.x - b.x, a.y - b.y, a.z - b.z};
    }
    operator *(a: Vector3, b: Vector3) : Vector3 {
        return Vector3{a.x * b.x, a.y * b.y, a.z * b.z};
    }
    operator /(a: Vector3, b: Vector3) : Vector3 {
        return Vector3{a.x / b.x, a.y / b.y, a.z / b.z};
    }

    operator *(a: Vector3, f: float) : Vector3 {
        return Vector3{a.x * f, a.y * f, a.z * f};
    }
}
proc operator_overloading() {
    print_string("\n\n---- operator_overloading ----\n");

    var v1 = Vector3{1, 2, 3};
    var v2 = Vector3{1, 4, 9};
    var v3 = v1 + v2;
    var v4 = v3 * 5;
    print_float(v4.x);
    print_float(v4.y);
    print_float(v4.z);
    assert(v4.x == 10);
    assert(v4.y == 30);
    assert(v4.z == 60);
}



// $ on the name of a parameter means this parameter MUST be constant
// and a new polymorph of the procedure will be baked with that constant.
// for example if you called `value_poly(4)` a new version of `value_poly`
// would be generated that replaces each use of the parameter `a` with the
// `4` that you passed. this means the return statement will constant fold
// into a simple `return 16;`
proc value_poly($a: int) : int {
    return a * a;
}
// $ on the type of a parameter means that this parameter can be ANY
// type, and a new version of this procedure will be generated for
// that type. for example `type_poly(124)` would generate a version
// of `type_poly` where `T` is `int`. `type_poly(Vector3{1, 4, 9})`
// would generate a version where `T` is `Vector3`
proc type_poly(a: $T) : T {
    return a * a;
}
// $ on both the name and the type means that this procedure can accept
// any type and it must be a constant. simply a combination of the two
// cases above.
proc value_and_type_poly($a: $T) : T {
    return a * a;
}
proc procedural_polymorphism() {
    print_string("\n\n---- procedural_polymorphism ----\n");

    print_int(value_poly(2));
    assert(value_poly(2) == 4);

    print_int(type_poly(3));
    assert(type_poly(3) == 9);
    print_float(type_poly(4.0));
    assert(type_poly(4.0) == 16);

    var a: int = 5;
    var f: float = 6;
    print_int(type_poly(a));
    print_float(type_poly(f));
    assert(type_poly(a) == 25);
    assert(type_poly(f) == 36);

    print_int(value_and_type_poly(7));
    print_float(value_and_type_poly(8.0));
    assert(value_and_type_poly(7) == 49);
    assert(value_and_type_poly(8.0) == 64);
}



// same with procedural polymorphism, a new version of the struct
// Custom_Array_Type will be generated for each set of parameters
// that are passed, creating a brand new type.
// for structs, unlike procedures, these values must all be constant
// values in order to maintain compile-time typesafety
struct Custom_Array_Type!($N: int, $T: typeid) {
    var array: [N]T;
    operator [](my_array: >Custom_Array_Type!(N, T), index: int) : >T {
        return my_array.array[index]; // implicitly takes a pointer to `my_array.array[index]` since this returns a reference
    }
}
proc structural_polymorphism() {
    print_string("\n\n---- structural_polymorphism ----\n");

    var array_of_ints: Custom_Array_Type!(8, int);
    array_of_ints[4] = 124; // calls [] operator overload
    print_int(array_of_ints[4]);
    assert(array_of_ints[4] == 124);
}



// the following is a barebones implementation of a resizable array
// using many of the features you've seen thus far including
// structural and procedural polymorphism and operator overloading

struct Dynamic_Array!($T: typeid) {
    var array: []T;
    var count: int;
    operator [](dyn: >Dynamic_Array!(T), index: int) : >T {
        return dyn.array[index];
    }
}

proc append(dyn: ^Dynamic_Array!($T), value: T) {
    if (dyn.count == dyn.array.count) {
        var old_data = dyn.array.data;
        var new_cap = 8 + dyn.array.count * 2;
        dyn.array.data = cast(^T, alloc(new_cap * sizeof(T)));
        dyn.array.count = new_cap;
        if (old_data != null) {
            memcpy(dyn.array.data, old_data, cast(u32, dyn.count * sizeof(T)));
            free(old_data);
        }
    }
    assert(dyn.count < dyn.array.count);
    dyn.array[dyn.count] = value;
    dyn.count += 1;
}

proc pop(dyn: ^Dynamic_Array!($T)) : T {
    assert(dyn.count > 0);
    var value = dyn[dyn.count-1];
    dyn.count -= 1;
    return value;
}

proc clear_dynamic_array(dyn: ^Dynamic_Array!($T)) {
    dyn.count = 0;
}

proc destroy_dynamic_array(dyn: Dynamic_Array!($T)) {
    if (dyn.array.data != null) {
        free(dyn.array.data);
    }
}

proc dynamic_arrays() {
    print_string("\n\n---- dynamic_arrays ----\n");

    var dyn: Dynamic_Array!(Vector3);
    append(&dyn, Vector3{1, 2, 3});
    append(&dyn, Vector3{1, 4, 9});
    append(&dyn, Vector3{2, 8, 18});
    assert(dyn[1].x == 1);
    assert(dyn[1].y == 4);
    assert(dyn[1].z == 9);
    for (var i = 0; i < dyn.count; i += 1) {
        print_int(i);
        print_float(dyn[i].x);
        print_float(dyn[i].y);
        print_float(dyn[i].z);
    }
    destroy_dynamic_array(dyn);
}
```