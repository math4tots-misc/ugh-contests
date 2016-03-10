// ********* Just quick lib *********
fn readln() -> String {
    let mut s = String::new();
    assert!(std::io::stdin().read_line(&mut s).is_ok());
    s.trim().to_string()
}

fn conv<T>(s: &str) -> T where
        T: std::str::FromStr,
        <T as std::str::FromStr>::Err: std::fmt::Debug {
    s.parse().expect("parse")
}

fn split<T>(s: String) -> Vec<T> where
        T: std::str::FromStr,
        <T as std::str::FromStr>::Err: std::fmt::Debug {
    s.split_whitespace().map(conv).collect()
}
// ********* end lib *********

fn main() {
    let t = conv::<usize>(&readln().trim());
    for _ in 0..t {
        let nm: Vec<usize> = split(readln());
        let n = nm[0];
        let m = nm[1];
        let mut e: Vec<Vec<(usize, i32)>> = Vec::new();
        e.resize(n, Vec::new());
        for _ in 0..m {
            let xyv: Vec<i32> = split(readln());
            let x = xyv[0] as usize - 1;
            let y = xyv[1] as usize - 1;
            let v = xyv[2];
            e[x].push((y, v));
            e[y].push((x, v));
        }
        let s = conv::<usize>(&readln()) - 1;
        let mut d: Vec<i32> = Vec::new();
        d.resize(n, -1);
        let mut q = std::collections::BinaryHeap::new();
        q.push((s, 0));
        loop {
            match q.pop() {
                Some((node, estimate)) => {
                    if d[node] == -1 || estimate < d[node] {
                        d[node] = estimate;
                        for &(neighbor, cost) in &e[node] {
                            q.push((neighbor, estimate + cost));
                        }
                    }
                }
                None => break
            }
        }
        for node in 0..n {
            if node != s {
                print!("{} ", d[node]);
            }
        }
        println!("");
    }
}
