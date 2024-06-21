# Functions

Functions in gooscript are first class data types.

```rust
fn square(x) {
    return x * x
}
```

One like functions can be declared with the use of the `=>` operator,
where the expression following the arrow will be what the function evaluates to.

```rust
fn square(x) => x * x

print(square(2)) 
```

To create anonymous functions (lambdas), you can simply omit
the name of the function.

```rust 
fn apply(arg, func) {
    func(arg)
}

apply(2, fn(x) => x * x)

```

> Internal Note:
>
> Under the hood, function statements are simplified down to a variable with a lambda.
>
> ```rust
> fn square(x) => x * x
>```
> Simplifies down to:
> ```rust
> let square = fn(x) => x * x
>```

For both statement-level functions (named functions) and lambdas, if
there are no arguments then you can omit the empty `()`.

```rust 
fn conditional(condition, true_func, false_func) => 
    if condition then true_func() else false_func()


conditional(
    true, 
    fn => print("true"),
    fn => print("false")
)
```