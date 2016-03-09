fn main() {
    let mut buf = String::new();
    assert!(std::io::stdin().read_line(&mut buf).is_ok());
    let x: i32 = buf.trim().parse().expect("parse x");
    println!("{}", x%11);
}
