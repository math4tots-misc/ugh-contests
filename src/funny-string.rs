//! funny-string.rs

fn diff(a: char, b: char) -> i32 {
    (a as i32 - b as i32).abs()
}

fn is_funny(text: &String) -> bool {
    let s: Vec<char> = text.chars().collect();
    let r = {
        let mut c = s.clone();
        c.reverse();
        c
    };
    for i in 1..s.len() {
        if diff(s[i], s[i-1]) != diff(r[i], r[i-1]) {
            return false;
        }
    }
    true
}

fn readln() -> String {
    let mut s = String::new();
    assert!(std::io::stdin().read_line(&mut s).is_ok());
    s.trim().to_string()
}

fn main() {
    let t = readln().parse::<usize>().expect("parse t");
    for _ in 0..t {
        println!("{}Funny", if is_funny(&readln()) {""} else {"Not "} );
    }
}
