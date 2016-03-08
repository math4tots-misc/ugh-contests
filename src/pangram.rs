//! pangram.rs

use std::io::Read;

fn main() {
    let text = {
        let mut text = String::new();
        assert!(std::io::stdin()
            .read_to_string(&mut text).is_ok());
        text.to_uppercase()
    };
    let mut seen = [false; 26];
    for c in text.chars() {
        if c.is_alphabetic() {
            seen[c as usize - 'A' as usize] = true;
        }
    }

    // Not sure I understand why the closure '|x| x' is not
    // interpreted as Fn.
    // No idea why this works with the '&' there.
    // TODO: Figure out why this works.
    let pangram = seen.into_iter().all(|&x| x );

    println!("{}pangram", if pangram { "" } else { "not "});
}
