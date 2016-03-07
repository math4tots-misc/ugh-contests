//! vertical-rooks.rs
//! Problem reduces to nim in a natural way.

use std::io::prelude::*;

fn readln() -> String {
    let mut line = String::new();
    assert!(std::io::stdin().read_line(&mut line).is_ok());
    return line;
}

fn ti(text: &String) -> i32 {
    return text.trim().parse().expect(text);
}

fn main() {
    let t = ti(&readln());
    let mut a: [i32; 2000] = [0; 2000];
    for _ in 0..t {
        let n = ti(&readln());
        for i in 0..n {
            a[i as usize] = ti(&readln());
        }
        let mut nimsum = 0;
        for i in 0..n {
            let x = ti(&readln());
            nimsum ^= (x-a[i as usize]).abs()-1;
        }
        println!("player-{}", if nimsum > 0 { 2 } else { 1 });
    }
}
