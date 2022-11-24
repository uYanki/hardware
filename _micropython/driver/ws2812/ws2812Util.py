class WS2812DrawUtil:
    def __init__(self,bufferWidth,bufferHeight):
        self.drawBuf=[]
        self.bw=bufferWidth
        self.bh=bufferHeight
        for i in range(bufferHeight):
            self.drawBuf.append([])
            for j in range(bufferWidth):
                self.drawBuf[i].append([0,0,0])
    
    def drawImg(self,imgData,x,y):
        for i in range(y,y+len(imgData)):
            for j in range(x,x+len(imgData[0])):
               if i>=self.bh or j>=self.bw: continue
               self.drawBuf[i][j][0]=imgData[i-y][j-x][0]
               self.drawBuf[i][j][1]=imgData[i-y][j-x][1]
               self.drawBuf[i][j][2]=imgData[i-y][j-x][2]
    
    def show(self,screen,bright=1):
        if bright>1: bright=1
        for i in range(self.bw):
            for j in range(self.bh):
                index=i*self.bh+((i%2)*self.bh+j*((-1)**i))-1*(i%2)
                screen[index]=(int(self.drawBuf[j][i][0]*bright),int(self.drawBuf[j][i][1]*bright),int(self.drawBuf[j][i][2]*bright))
        screen.write()

