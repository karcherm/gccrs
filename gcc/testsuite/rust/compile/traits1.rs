trait Foo {
    fn Bar() -> i32 {}
}

struct Baz;

impl Foo for Baz {
    fn Barrr() {}
    // { dg-error "method .Barrr. is not a member of trait .Foo." "" { target *-*-* } .-1 }
}

fn main() {}
