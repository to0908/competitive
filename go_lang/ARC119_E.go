package main

import (
	"bufio"
	"container/heap"
	"fmt"
	// "math"
	"os"
	"reflect"
	"sort"
	"strconv"
)

type UnionFind struct {
	par []int
}
func NewUnionFind(n int) UnionFind{
	u := UnionFind{make([]int,n)}
	for i:=0;i<n;i++{
		u.par[i] = -1
	}
	return u
}
func (u *UnionFind) root(a int) int{
	if u.par[a] < 0{
		return a
	}
	u.par[a] = u.root(u.par[a])
	return u.par[a]
}
func (u *UnionFind) size(a int) int{
	return -u.par[u.root(a)]
}
func (u *UnionFind) same(a,b int) bool{
	return u.root(a)==u.root(b)
}
func (u *UnionFind) unite(a,b int) bool{
	a = u.root(a)
	b = u.root(b)
	if a == b{
		return false
	}
	if u.size(a) < u.size(b){
		a,b = b,a
	}
	u.par[a] += u.par[b]
	u.par[b] = a
	return true
}

func reverse(a []int) []int{
	ret := make([]int,len(a))
	for i:=0;i<len(a);i++{
		ret[i] = a[len(a)-1-i]
	}
	return ret;
}

func solve(a []int) int {
	ret := 0
	pq := new(PriorityQueue)
	for i:=1;i<len(a);i++{
		if a[i] >= a[i-1]{
			heap.Push(pq,State{a[i-1],a[i]})
		}
		chmax(&ret,IntAbs(a[i] - a[i-1]) - IntAbs(a[i-1]-a[len(a)-1]))
	}
	rma := 0
	for ;pq.Len()>0;{
		cur := heap.Pop(pq).(State)
		l := cur.dist
		r := cur.p
		// fmt.Println(rma,l,r)
		chmax(&ret,(IntMin(rma,r)-l)*2)
		chmax(&rma,r)
	}
	// fmt.Println("ans:",ret)
	return ret
}

func IntMin(a,b int) int{
	if a>b{return b}else{return a}
}
func IntAbs(a int) int{
	if a<0 {return -a}else{return a}
}

func main() {
	// fmt.Fscan(read,&hoge,&fuga,...)
	// fmt.Fprintln(write,hoge,fuga,...)
	write := bufio.NewWriter(os.Stdout)
	defer write.Flush()
	// read := bufio.NewReader(os.Stdin)　// <-どちらか一方のみ 
	buf := make([]byte, 1024*1024)
	sc.Buffer(buf, bufio.MaxScanTokenSize)
	sc.Split(bufio.ScanWords)

	n := readInt()
	a := make([]int,n+2)
	for i:=1;i<=n;i++{
		a[i] = readInt()
	}
	a[0] = a[1]
	a[n+1] = a[n]
	ans := solve(a)
	chmax(&ans,solve(reverse(a)))
	ans = -ans
	for i:=1;i<=n;i++{
		if a[i] > a[i-1]{
			ans += a[i] - a[i-1]
		}else{
			ans += a[i-1] - a[i]
		}
	}
	fmt.Fprintln(write,ans)


}


// io -------------------------
var sc = bufio.NewScanner(os.Stdin)
func readInt() int {
	sc.Scan()
	i, _ := strconv.Atoi(sc.Text())
	return i
}
// --------------------------------

// chmax(&a,b)として使う
func chmax(a *int, b int) bool {
	if *a>=b{
		return false
	}else{
		*a=b
		return true
	}
}

func chmin(a *int, b int) bool {
	if *a<=b{
		return false
	}else{
		*a=b
		return true
	}
}

// ソートされたkeyを返す。keyの型だけ指定する必要がある
func MapSortingString(mp interface{}) []string{
	keys := reflect.ValueOf(mp).MapKeys()
    res := make([]string, len(keys))
    for i, key := range keys {
        res[i] = key.String()
    }
    sort.Strings(res)
    return res
}
func MapSortingInt(mp interface{}) []int{
	keys := reflect.ValueOf(mp).MapKeys()
    res := make([]int, len(keys))
    for i, key := range keys {
        res[i] = int(key.Int())
    }
    sort.Ints(res)
    return res
}

type Array struct{
	// a,b int
	dist float64
	x,y int
	// HOW TO SORT
	// x := make([]Array,n)
	// sort.Slice(x, func(i,j int) bool {
	// 	return x[i].a < x[j].a
	// })
}

type Edge struct {
	to, cost int
}
type WeightedGraph struct {
	n int
	v [][]Edge
}
func NewWeightedGraph(n int) WeightedGraph {
	g := WeightedGraph{n,make([][]Edge,n)}
	return g
}
func (g *WeightedGraph) add_directed_edge(from, to, cost int){
	g.v[from] = append(g.v[from],Edge{to,cost})
}
func (g *WeightedGraph) add_undirected_edge(u, v, cost int){
	g.v[u] = append(g.v[u],Edge{v,cost})
	g.v[v] = append(g.v[v],Edge{u,cost})
}
type State struct { dist, p int }
type PriorityQueue []State
func (pq PriorityQueue) Len() int { return len(pq) }
func (pq PriorityQueue) Less(i, j int) bool { return pq[i].dist < pq[j].dist }
func (pq PriorityQueue) Swap(i, j int) { pq[i], pq[j] = pq[j], pq[i] }
func (pq *PriorityQueue) Push(x interface{}) { *pq = append(*pq, x.(State)) }
func (pq *PriorityQueue) Pop() interface{} {
	x := (*pq)[len(*pq)-1]
	*pq = (*pq)[0 : len(*pq)-1]
	return x
}
func (g *WeightedGraph) shortest_path(s int)[]int{
	n := g.n
	INF := 100000000000000000+7
	dp := make([]int,n)
	for i:=0;i<n;i++ {
		dp[i] = INF
	}
	pq := new(PriorityQueue)
	heap.Push(pq,State{0,s})
	for pq.Len() != 0 {
		cur := heap.Pop(pq).(State)
		if(dp[cur.p] < cur.dist){ continue }
		for i:=0;i<len(g.v[cur.p]);i++{
			nv := g.v[cur.p][i].to
			ndist := cur.dist + g.v[cur.p][i].cost
			if dp[nv] > ndist{
				dp[nv] = ndist
				heap.Push(pq,State{ndist,nv})
			}
		}
	}
	return dp
}
