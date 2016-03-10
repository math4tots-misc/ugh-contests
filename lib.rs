// Just quick lib
fn readln() -> String {
    let mut s = String::new();
    assert!(std::io::stdin().read_line(&mut s).is_ok());
    s.trim().to_string()
}

fn conv<T>(s: &str) ->
        T where T: std::str::FromStr,
        <T as std::str::FromStr>::Err: std::fmt::Debug {
    s.parse().expect("parse")
}

fn split(s: String) -> Vec<i32> {
    s.split_whitespace().map(conv).collect()
}

// Just to see if it compiles
// Don't copy the parts after here.

fn main() {}
