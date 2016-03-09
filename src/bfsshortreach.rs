fn readln() -> String {
    let mut s = String::new();
    assert!(std::io::stdin().read_line(&mut s).is_ok());
    s.trim().to_string()
}

fn ti(s: &str) -> usize {
    s.parse().unwrap()
}

fn read2i() -> (usize, usize) {
    let line = readln();
    let mut pair = line.split_whitespace().map(ti);
    let first = pair.next().unwrap();
    let second = pair.next().unwrap();
    (first, second)
}

fn main() {
    let t = ti(&readln());
    for _ in 0..t {
        let (n, m) = read2i();
        let mut e: Vec<Vec<usize>> = Vec::new();
        e.resize(n, Vec::new());
        for _ in 0..m {
            let (a, b) = read2i();
            e[a-1].push(b-1);
            e[b-1].push(a-1);
        }
        let s = ti(&readln())-1;
        let mut d: Vec<i32> = Vec::new();
        d.resize(n, -1);
        let mut q: std::collections::VecDeque<usize> =
                std::collections::VecDeque::new();
        q.push_back(s);
        d[s] = 0;
        loop {
            match q.pop_front() {
                Some(node) => {
                    for &neighbor in &e[node] {
                        if d[neighbor] == -1 {
                            d[neighbor] = d[node] + 6;
                            q.push_back(neighbor);
                        }
                    }
                }
                None => break
            }
        }
        for node in 0..n {
            if node != s {
                print!("{} ", d[node])
            }
        }
        println!("");
    }
}
