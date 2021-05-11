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
	var dp [2][101]int
	x := 0
	for i:=0;i<2;i++{
		for j:=0;j<100;j++{
			dp[i][j]=-1e18
		}
	}
	dp[1][0]=0;
	for i:=0;i<n;i++{
		for j:=0;j<100;j++{
			dp[x][j] = -1e18
		}
		dp[x][0] = 0
		a := readInt()
		b := readInt()
		for j:=0;j<100;j++{
			chmax(&dp[x][(j+a)%100], dp[x^1][j] + b - a + int((j+a)/100)*20);
			chmax(&dp[x][j],dp[x^1][j])
		}
		x ^= 1
	}
	ans := 0
	x ^= 1
	for i:=0;i<100;i++{
		chmax(&ans,dp[x][i])
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
	a,b int
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
type Dijkstra []State
func (pq Dijkstra) Len() int { return len(pq) }
func (pq Dijkstra) Less(i, j int) bool { return pq[i].dist < pq[j].dist }
func (pq Dijkstra) Swap(i, j int) { pq[i], pq[j] = pq[j], pq[i] }
func (pq *Dijkstra) Push(x interface{}) { *pq = append(*pq, x.(State)) }
func (pq *Dijkstra) Pop() interface{} {
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
	pq := new(Dijkstra)
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
