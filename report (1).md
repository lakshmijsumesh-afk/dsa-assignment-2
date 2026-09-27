# Hash Table vs Linear Search — Song ID Lookup
### Data Structures Lab Exercise

---

## 1. Problem & Input Data

**Song IDs stored by the music application (insertion order):**

```
105, 210, 315, 420, 525, 630, 735, 840
```
n = 8 song IDs.

**Search queries used in Part (b):**

```
420   -> present (6th... 4th inserted, middle of a chain)
735   -> present (near end of a long chain)
105   -> present (first item inserted, head of a chain)
999   -> absent (tests unsuccessful search)
```

---

## 2. Part (a): Insertion (Division Method, table size = 10, h(key) = key mod 10)

| Step | ID inserted | h(key)=key mod 10 | Collision? | Bucket 0 | Bucket 5 |
|---|---|---|---|---|---|
| 1 | 105 | 5 | No  | – | 105 |
| 2 | 210 | 0 | No  | 210 | 105 |
| 3 | 315 | 5 | **Yes** (bucket 5 occupied) | 210 | 105→315 |
| 4 | 420 | 0 | **Yes** (bucket 0 occupied) | 210→420 | 105→315 |
| 5 | 525 | 5 | **Yes** | 210→420 | 105→315→525 |
| 6 | 630 | 0 | **Yes** | 210→420→630 | 105→315→525 |
| 7 | 735 | 5 | **Yes** | 210→420→630 | 105→315→525→735 |
| 8 | 840 | 0 | **Yes** | 210→420→630→840 | 105→315→525→735 |

**Final table (all other buckets 1,2,3,4,6,7,8,9 = EMPTY):**
```
[0] -> 210 -> 420 -> 630 -> 840
[5] -> 105 -> 315 -> 525 -> 735
```
**Total collisions = 6 out of 8 insertions. Load factor α = n/m = 8/10 = 0.80**

**Why so many collisions?** Every song ID is a multiple of 105, and 105 mod 10 = 5. Since gcd(105,10)=5, every key's remainder mod 10 is either 0 or 5 — the data is not "random" relative to this table size, so the division method degenerates into essentially 2 buckets holding 4 keys each.

---

## 3. Part (b): Search comparisons

| Key | Present? | Hash Search comparisons | Linear Search comparisons |
|---|---|---|---|
| 420 | Yes | 3 | 4 |
| 735 | Yes | 5 | 7 |
| 105 | Yes | 2 | 1 |
| 999 | No  | 1 | 8 |
| **Total** | | **11** | **20** |
| **Average** | | **2.75** | **5.00** |

(Hash-search comparison count = 1 for computing the index + 1 per node visited in that bucket's chain until the key is found/chain ends. Linear-search count = number of array elements inspected left-to-right.)

---

## 4. Bonus experiment — same keys, prime table size 11 (h(key)=key mod 11)

```
105→6, 210→1, 315→7, 420→2, 525→8, 630→3, 735→9, 840→4
```
**Result: 0 collisions**, every key lands in its own bucket. Load factor = 8/11 = 0.73.

This isolates the real cause of the collisions in Part (a): it is not "hashing" that is weak, it is the **choice of table size 10** combined with the specific structure of this data (all multiples of 105). A prime table size that shares no common factor with the key pattern removes the clustering entirely.

---

## 5. Complexity Analysis

| Operation | Hashing (chaining) | Linear Search |
|---|---|---|
| **Best case** | O(1) — key is alone in its bucket | O(1) — key is first element |
| **Average case** | O(1 + α) where α = load factor | O(n) |
| **Worst case** | O(n) — all keys collide into one bucket (as nearly happens in Part a) | O(n) |
| **Space** | O(n + m) — n nodes + m bucket headers (m=10) | O(n) — array only |
| **Insertion** | O(1) average, O(chain length) worst | O(1) (append) but no ordering benefit |

- With table size 10 and α = 0.8, the *theoretical* average number of probes for a successful chained-hash search is ≈ 1 + α/2 = 1.4, and for an unsuccessful search ≈ 1 + α = 1.8.
- **Observed** average was 2.75 for successful/mixed searches — higher than the generic chaining formula predicts, because the actual key distribution is *not uniform* (all 8 keys fell into only 2 of the 10 buckets instead of spreading out), so real chain lengths (4) are much longer than the "average" chain length (0.8) the formula assumes.
- Linear search's observed average (5.00 comparisons) is consistent with the theoretical expectation of ~(n+1)/2 ≈ 4.5 for successful search and n=8 for unsuccessful search.

---

## 6. Comparison Table — Hashing vs Linear Search (this dataset)

| Criterion | Hashing (Division Method, size 10) | Linear Search |
|---|---|---|
| Time complexity (avg) | O(1) in theory, degraded here by clustering | O(n) |
| Time complexity (worst) | O(n) | O(n) |
| Observed avg comparisons | 2.75 | 5.00 |
| Collisions during build | 6 of 8 insertions | N/A |
| Load factor | 0.80 (high) | N/A |
| Space overhead | Extra pointers/buckets (O(n+m)) | None beyond the array |
| Sensitive to key pattern? | Yes — very sensitive to table size vs key structure | No — performance is independent of key values |
| Performance with size-11 table | 0 collisions, ~O(1) real performance | unchanged |

---

## 7. Conclusion

1. Hashing (size 10) still **outperformed linear search on this dataset** (2.75 vs 5.00 average comparisons, and dramatically so for the unsuccessful search: 1 vs 8), even though 6 of the 8 insertions collided.
2. The collisions arose purely from a **poor table-size choice** (10 shares a factor of 5 with every key, since all IDs are multiples of 105), not from a flaw in hashing itself. This is confirmed by the bonus run: switching to a prime table size (11) eliminated every collision for the same data.
3. **Hashing is suitable for this application**, provided the table size is chosen well (a prime number not sharing factors with the ID-generation scheme, and sized so the load factor stays around 0.7 or below). With that fix, song ID lookup approaches true O(1), which matters for a music app doing frequent ID lookups (e.g., "now playing" queries, library scans).
4. Linear search remains simple and needs no extra memory, and is acceptable only if the song library stays very small (a few dozen entries) or is rarely searched; it does not scale as the catalog grows, since its cost grows linearly with catalog size regardless of table design.
5. **Recommendation:** use hashing with a prime table size sized to keep load factor ≤ ~0.7, and prefer chaining (or open addressing with a secondary hash) as the collision-resolution strategy for this ID pattern.
