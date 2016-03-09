fn main() {
    let mut s = String::new();
    assert!(std::io::stdin().read_line(&mut s).is_ok());
    let x: i32 = s.parse().unwrap();
    println!("{}", x * x);
}
