""" """

import argparse
import copy
from dataclasses import dataclass, field
import sys
import time
import tkinter as tk
import tkinter.ttk
import tkinter.messagebox as tkm
from PIL import Image

def parse_arguments():
    parser = argparse.ArgumentParser(description=' ')
    parser.add_argument('-i', '--input_file', required=True)
    parser.add_argument('-o', '--output_file', required=True)
    args = parser.parse_args()
    return args

W = 700
H = 700

def ri(f):
    return int(f.readline())

def rf(f):
    return float(f.readline())

class Input:
    def __init__(self):
        pass
    
    def load(self, filepath):
        with open(filepath) as f:
            self.N = ri(f)
            self.V = ri(f)
            self.B = rf(f)
            self.grid = []
            for r in range(self.N):
                row = [ri(f) for c in range(self.N)]
                self.grid.append(row)
            # self.show()
    
    def show(self):
        for r in range(self.N):
            print(self.grid[r])

dice_ = [
[0,1,2,3,4,5],
[2,3,1,0,4,5],
[1,0,3,2,4,5],
[3,2,0,1,4,5],
[5,4,0,1,3,2],
[0,1,4,5,3,2],
[4,5,1,0,3,2],
[1,0,5,4,3,2],
[2,3,5,4,1,0],
[5,4,3,2,1,0],
[3,2,4,5,1,0],
[4,5,2,3,1,0],
[1,0,2,3,5,4],
[2,3,0,1,5,4],
[0,1,3,2,5,4],
[3,2,1,0,5,4],
[5,4,1,0,2,3],
[1,0,4,5,2,3],
[4,5,0,1,2,3],
[0,1,5,4,2,3],
[3,2,5,4,0,1],
[5,4,2,3,0,1],
[2,3,4,5,0,1],
[4,5,3,2,0,1],
]
trans_ = [
[1,3,21,11],
[2,0,16,6],
[3,1,9,23],
[0,2,4,18],
[5,7,13,3],
[6,4,22,10],
[7,5,1,15],
[4,6,8,20],
[9,11,19,7],
[10,8,14,2],
[11,9,5,17],
[8,10,0,12],
[13,15,11,21],
[14,12,18,4],
[15,13,23,9],
[12,14,6,16],
[17,19,15,1],
[18,16,10,22],
[19,17,3,13],
[16,18,20,8],
[21,23,7,19],
[22,20,12,0],
[23,21,17,5],
[20,22,2,14],
]


UP = 0
DOWN = 1
LEFT = 2
RIGHT = 3
DX = [0, 0, -1, 1]
DY = [-1, 1, 0, 0]

@dataclass
class Pos:
    x: int = -1
    y: int = -1
    d: int = -1
    def to(self, dir: int):
        assert self.d != -1
        return Pos(self.x+DX[dir], self.y+DY[dir], trans_[self.d][dir])

def getDir(cur: Pos, nex: Pos):
    if cur.x > nex.x: return LEFT
    if cur.x < nex.x: return RIGHT
    if cur.y > nex.y: return UP
    if cur.y < nex.y: return DOWN
    assert False

@dataclass
class Output:
    dice: list[int] = field(default_factory=list)
    moves: list[Pos] = field(default_factory=list)
    score: int = 0

    def load(self, lines: list[str]):
        # 読み込んだ行数を返す
        n_line = 0
        self.dice = list(map(int, lines[:6]))
        n_line += 6
        n_move = int(lines[n_line])
        n_line += 1

        prev = None
        for i in range(n_move):
            y, x = map(int, lines[n_line].strip().split(' '))
            n_line += 1
            if i == 0:
                cur = Pos(x, y, 0)
            else:
                cur = Pos(x, y)
                dir = getDir(prev, cur)
                cur = prev.to(dir)
                assert cur.x == x
                assert cur.y == y
            self.moves.append(cur)
            prev = cur
        return n_line
    
    def getBottom(self, mid: int):
        # moves[mid]のBOTTOMのdiceの値を返す
        # print(self.moves[mid])
        # for i in range(6):
        #     print(self.dice[dice_[self.moves[mid].d][i]])
        return self.dice[dice_[self.moves[mid].d][1]]

    def calcScore(self, input: Input):
        ret = 0
        for i, move in enumerate(self.moves):
            d = self.getBottom(i)
            v = input.grid[move.y][move.x]
            if abs(v) == d:
                ret += v
        self.score = ret
        return ret


class Outputs:
    def __init__(self):
        self.oid = 0
        self.outputs = []  # list[Output]
    
    def load(self, filepath):
        with open(filepath) as f:
            lines = f.readlines()
            offset = 0
            while True:
                output = Output()
                offset += output.load(lines[offset:])
                self.outputs.append(output)
                if offset == len(lines):
                    break
    
    def get(self):
        return self.outputs[self.oid]
    
    def next(self):
        self.oid = min(len(self.outputs)-1, self.oid+1)

    def prev(self):
        self.oid = max(0, self.oid-1)

    def first(self):
        self.oid = 0

    def last(self):
        self.oid = len(self.outputs)-1


class Application(tkinter.Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.master = master
        self.master.title('tkinter canvas trial')
        self.master.geometry('1200x800')
        self.pack()
        self.create_widgets()
        self.input = Input()
        self.output = Outputs()
    
    def load_input(self, filepath):
        self.input.load(filepath)

    def load_output(self, filepath):
        self.output.load(filepath)
        for out in self.output.outputs:
            out.calcScore(self.input)

    def draw(self):
        self.canvas.delete("all")
        self.draw_output()
        self.draw_input()
        self.canvas.update()
        self.infoCanvas.delete("all")
        self.draw_info()
        self.infoCanvas.update()

    def draw_input(self):
        size = W//self.input.N
        cur = self.output.get()
        for y in range(self.input.N):
            for x in range(self.input.N):
                v = self.input.grid[y][x]
                if v in cur.dice and (x,y) not in self.used:
                    self.canvas.create_rectangle(x*size, y*size, (x+1)*size, (y+1)*size, fill = 'Blue')
                self.canvas.create_text(x*size+size//2, y*size+size//2, text=str(self.input.grid[y][x]), font=("Helvetica", size//2))

    def draw_output(self):
        size = W//self.input.N
        cur = self.output.get()
        pmove = None
        self.used = set()
        for i, move in enumerate(cur.moves):
            d = cur.getBottom(i)
            x = move.x
            y = move.y
            v = self.input.grid[y][x]
            if abs(v) == d:
                self.used.add((x,y))
                if v > 0:
                    color = 'Green'
                else:
                    color = 'Red'
                self.canvas.create_rectangle(x*size, y*size, (x+1)*size, (y+1)*size, fill = color)
            if pmove is not None:
                self.canvas.create_line(
                    pmove.x*size+size//2,
                    pmove.y*size+size//2,
                    move.x*size+size//2,
                    move.y*size+size//2,
                )
            pmove = move

    def draw_info(self):
        option = {
            "font": ("Helvetica", 10),
            "anchor": "w",
        }
        h = 10
        self.infoCanvas.create_text(0, h, text=f"N={self.input.N} V={self.input.V} B={self.input.B:.3f}", **option)
        h += 15
        h += 15
        # outputの情報
        self.infoCanvas.create_text(0, h, text=f"step = {self.output.oid+1} / {len(self.output.outputs)}", **option)
        h += 15
        cur = self.output.get()
        self.infoCanvas.create_text(0, h, text=f"dice = {''.join(map(str,cur.dice))}", **option)
        h += 15
        self.infoCanvas.create_text(0, h, text=f"score = {cur.score} ({cur.score*self.input.B:.1f})", **option)

    def create_widgets(self):
        self.canvas = tk.Canvas(self, width = W, height = H)
        self.canvas.grid(row=0, column=0, rowspan=3)
        self.canvas.update()

        self.first_button = tkinter.ttk.Button(self, text='<<')
        self.first_button.grid(row=1, column=1)
        self.first_button.bind('<Button-1>', self.first)

        self.prev_button = tkinter.ttk.Button(self, text='<')
        self.prev_button.grid(row=1, column=2)
        self.prev_button.bind('<Button-1>', self.prev)

        self.run_button = tkinter.ttk.Button(self, text='r')
        self.run_button.grid(row=1, column=3)
        self.run_button.bind('<Button-1>', self.run)

        self.next_button = tkinter.ttk.Button(self, text='>')
        self.next_button.grid(row=1, column=4)
        self.next_button.bind('<Button-1>', self.next)

        self.last_button = tkinter.ttk.Button(self, text='>>')
        self.last_button.grid(row=1, column=5)
        self.last_button.bind('<Button-1>', self.last)


        self.infoCanvas = tk.Canvas(self, width = 200, height = 100)
        self.infoCanvas.grid(row=0, column=1, columnspan=5)
    
    def next(self, event):
        self.output.next()
        self.draw()

    def run(self, event):
        while self.output.oid < len(self.output.outputs)-1:
            self.output.next()
            self.draw()
            time.sleep(0.1)

    def prev(self, event):
        self.output.prev()
        self.draw()

    def last(self, event):
        self.output.last()
        self.draw()

    def first(self, event):
        self.output.first()
        self.draw()



def main(args):
    # アプリケーション起動
    root = tk.Tk()
    app = Application(master=root)
    app.load_input(args.input_file)
    app.load_output(args.output_file)
    app.draw()
    app.mainloop()

    # n, edges = load_input(args.input_file)
    # mat_out = load_output(args.output_file)
    # matshow(mat_out)

    # # canvas.postscript(file="tmp.matrix.eps",colormode='color')
    # # save_as_png(args.image_file)


def save_as_png(filename):
    # save postscipt image 
    img = Image.open('tmp.matrix.eps') 
    img.save(filename, 'png') 


if __name__ == '__main__':
    args = parse_arguments()
    main(args)

