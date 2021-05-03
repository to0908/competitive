package main

import (
	"bufio"
	"fmt"
	"os"
	"reflect"
	"sort"
	"container/heap"
	"strconv"
)

type State struct {
	dist, p int
}

type Dijkstra []State
func (pq Dijkstra) Len() int {
	return len(pq)
}
func (pq Dijkstra) Less(i, j int) bool {
	return pq[i].dist < pq[j].dist	
}
func (pq Dijkstra) Swap(i, j int) {
	pq[i], pq[j] = pq[j], pq[i]
}
func (pq *Dijkstra) Push(x interface{}) {
	*pq = append(*pq, x.(State))
}
func (pq *Dijkstra) Pop() interface{} {
	x := (*pq)[len(*pq)-1]
	*pq = (*pq)[0 : len(*pq)-1]
	return x
}
type Edge struct {
	to, cost int
}
type WeightedGraph struct {
	n int
	v  [][]Edge
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

func (g *WeightedGraph) shortest_path(s,node int)[]int{
	n := g.n
	INF := 100000000000000000+7
	dp := make([]int,n)
	for i:=0;i<n;i++ {
		dp[i] = INF
	}
	pq := new(Dijkstra)
	heap.Push(pq,State{0,s})
	for pq.Len() != 0 {
		S := heap.Pop(pq).(State)
		if dp[S.p] < S.dist { continue }
		x := S.p%node
		for i:=0;i<len(g.v[x]);i++{
			nv := g.v[x][i].to + S.p - x
			if g.v[x][i].cost > 0{
				ndist := S.dist + g.v[x][i].cost
				if dp[nv] > ndist{ 
					dp[nv] = ndist
					heap.Push(pq,State{ndist,nv}) 
				}
			}else if nv+node<g.n{
				ndist := S.dist - g.v[x][i].cost
				if dp[nv+node] > ndist {
					dp[nv+node] = ndist
					heap.Push(pq,State{ndist,nv+node})
				}
			}
		}
	}
	return dp
}

func main() {
	// fmt.Fscan(read,&hoge,&fuga,...)
	// fmt.Fprintln(write,hoge,fuga,...)
	// read := bufio.NewReader(os.Stdin)
    // write := bufio.NewWriter(os.Stdout)
    // defer write.Flush()

	buf := make([]byte, 1024*1024)
	sc.Buffer(buf, bufio.MaxScanTokenSize)
	sc.Split(bufio.ScanWords)
	var (
		n int
		m int
		l int
	)
	// fmt.Fscan(read,&n,&m,&l)
	n=readInt()
	m=readInt()
	l=readInt()
	var v WeightedGraph = NewWeightedGraph(n*(m+1))
	for i:=0;i<m;i++{
		var a,b,c int
		// fmt.Fscan(read,&a,&b,&c)
		a=readInt()
		b=readInt()
		c=readInt()
		a--
		b--
		v.add_directed_edge(a,b,c)
		v.add_directed_edge(b,a,-c)
	}
	d := v.shortest_path(0,n)
	for i:=0;i<=m;i++{
		if d[i*n+n-1] <= l{
			fmt.Println(i)
			return
		}
	}
	fmt.Println(-1)
	
}

// input -------------------------
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
	a int
	b int
	// HOW TO SORT
	// x := make([]Array,n)
	// sort.Slice(x, func(i,j int) bool {
	// 	return x[i].a < x[j].a
	// })
}
