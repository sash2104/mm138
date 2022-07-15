dice = [0,1,2,3,4,5]
to = [
    [3,2,0,1,4,5],
    [2,3,1,0,4,5],
    [4,5,2,3,1,0],
    [5,4,2,3,0,1],
]

vocab = dict()
trans = [[0 for _ in range(4)] for _ in range(24)]

def hash(cur):
    return "".join(map(str,cur))

def dfs(cur):
    hcur = hash(cur)
    cid = vocab[hcur]
    for i in range(4):
        nex = [0 for _ in range(6)]
        for j in range(6):
            nex[to[i][j]] = cur[j]
        hnex = hash(nex)
        skip = hnex in vocab
        if not skip:
            vocab[hnex] = len(vocab)
        nid = vocab[hnex]
        trans[cid][i] = nid
        if not skip:
            dfs(nex)


vocab[hash(dice)] = len(vocab)
dfs(dice)

for k, v in vocab.items():
    print(f'{{{",".join(list(k))}}},')
for i in range(24):
    print(f'{{{",".join(map(str,trans[i]))}}},')