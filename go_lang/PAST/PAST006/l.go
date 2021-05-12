package main

import (
	"bufio"
	"container/heap"
	"fmt"
	"math"
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

func dist(x1,y1,x2,y2 float64) float64{
	return math.Sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2))
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
	m := readInt()
	p := make([][2]float64,n)
	c := make([][3]float64,m)
	for i:=0;i<n;i++{
		p[i][0] = float64(readInt())
		p[i][1] = float64(readInt())
	}
	for i:=0;i<m;i++{
		c[i][0] = float64(readInt())
		c[i][1] = float64(readInt())
		c[i][2] = float64(readInt())
	}
	ans := float64(1e18)
	for bit:=0;bit<(1<<m);bit++{
		uni := NewUnionFind(n+m)
		var v []Array
		for i:=0;i<n;i++{
			for j:=i+1;j<n;j++{
				d := dist(p[i][0],p[i][1],p[j][0],p[j][1])
				v = append(v, Array{d,i,j})
			}
			for j:=0;j<m;j++{
				if (bit & (1<<j))!=0 {
					d := dist(p[i][0],p[i][1],c[j][0],c[j][1])
					if d > c[j][2]{
						d -= c[j][2]
					}else{
						d = c[j][2]-d;
					}
					v = append(v, Array{d,i,j+n})
				}
			}
		}
		for i:=0;i<m;i++{
			if (bit & (1<<i)) != 0{
				for j:=i+1;j<m;j++{
					if (bit & (1<<j)) != 0{
						pd := dist(c[i][0],c[i][1],c[j][0],c[j][1])
						if pd > c[i][2] + c[j][2]{
							v = append(v, Array{pd-c[i][2]-c[j][2],i+n,j+n})
						}else if math.Abs(c[i][2] - c[j][2]) > pd {
							d := math.Abs(c[i][2] - c[j][2]) - pd
							v = append(v, Array{d,i+n,j+n})
						}else{
							uni.unite(i+n,j+n)
						}
					}
				}
			}
		}
		sort.Slice(v, func(i,j int) bool {
			return v[i].dist < v[j].dist
		})
		ret := float64(0)
		for i:=0;i<len(v);i++{
			if uni.unite(v[i].x,v[i].y){
				ret += v[i].dist
			}
		}
		ans = math.Min(ans,ret)
	}
	fmt.Println(fmt.Sprintf("%.20f",ans))
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
