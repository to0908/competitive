// problem: https://atcoder.jp/contests/utpc2012/tasks/utpc2012_01
// submission: https://atcoder.jp/contests/utpc2012/submissions/21779019
package main

import "fmt"

func main() {
  var s string
  fmt.Scan(&s)
  //fmt.Println(s)
  var l = len(s)
  //fmt.Println(l)
  var year = true
  cnt := [222]int{}
  for i := 0; i < l; i++{
    if(s[i]=='/'){
      year = false
      continue
    }
    if(year){
      cnt[s[i]-'0']++
    } else {
      cnt[s[i]-'0']--
    }
  }
  for i := 0; i < 222; i++{
    if(cnt[i]!=0){
      fmt.Println("no")
      return
    }
  }
  fmt.Println("yes")
}
