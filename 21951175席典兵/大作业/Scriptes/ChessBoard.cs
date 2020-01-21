using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

//类外定义，其他变量可以访问
//一处定义，其他不能再重复定义
public enum ChessType {Watch,Black,White};
public class ChessBoard : MonoBehaviour {
    public int[,] grid;//棋盘,用来储存该位置落子的情况，无子为0，B为1，W为2
    public GameObject[] prefabs;//棋子
    public float timer = 0;//下棋的间隔时间，意思是落子后要过timer的时间才能落子
    public bool gameStart = true;//下棋是否结束
    public ChessType turn =ChessType.Black;//表面当前下棋的棋子，用来进行先后手
    public GameObject parent;//父亲对象
    public Stack<GameObject> s=new Stack<GameObject>();//栈,如果未初始化，则之后压棋子有错
    public Text winner;
    //做成单例模式
    static ChessBoard _instance;
    public static ChessBoard Instance
    {
        get
        {
            return _instance;
        }
    }
    //对_instance赋值，赋值完后，通过Instance即可访问到_instance
    private void Awake()
    {
        if (Instance == null)
        {
            _instance = this;
        }
    }
   

     public bool PlayChess(int[] pos)
    {
        if (!gameStart)
        {
            print("对局已结束");
            return false;
        } 
        //对位置的坐标值进行限制
        pos[0] = Mathf.Clamp(pos[0], 0, 14);
        pos[1] = Mathf.Clamp(pos[1], 0, 14);
        //判断当前该位置是否可以下棋
        if (grid[pos[0], pos[1]] != 0) {
            return false;
        }
       
        //如果是黑子下
        if (turn == ChessType.Black)
        {
            //实例化黑子
            GameObject go=Instantiate(prefabs[0], new Vector3(pos[0] - 7, pos[1] - 7), Quaternion.identity);
            //棋子入栈 
            s.Push(go);
            go.transform.SetParent(parent.transform);
            //
            //该点已下黑子
            grid[pos[0], pos[1]] = 1;
            //判断胜负
            if (CheckWinner(pos)){
                GameEnd();
            }
            //如果未分胜负，则白子下棋
            turn = ChessType.White;
        }
        else if(turn==ChessType.White)
        {
            //实例化白子
            GameObject go=Instantiate(prefabs[1], new Vector3(pos[0] - 7, pos[1] - 7), Quaternion.identity);
            //棋子入栈
            s.Push(go);
            //设置该棋子的父亲
            go.transform.SetParent(parent.transform);
            //该点已下白子
            grid[pos[0], pos[1]] = 2;
            //判断胜负
            if (CheckWinner(pos))
            {
                GameEnd();
            }
            //如果未分胜负，则黑子下棋
            turn = ChessType.Black;
        }

        return true;
    }
    //检测胜负
    public bool CheckWinner(int[] pos)
    {
        if (CheckOneLine(pos, new int[2] { 1, 0 })) { return true; }
        if (CheckOneLine(pos, new int[2] { 0, 1 })) return true;
        if (CheckOneLine(pos, new int[2] { 1, 1 })) return true;
        if (CheckOneLine(pos, new int[2] { 1, -1 })) return true;
        return false;
    }
 
    //检测该方向是否形成5子连珠
    public bool CheckOneLine(int []pos,int []off)
    {
        int num = 1;
        //往右边扫
        //判断是否满足边界
        for (int i = off[0],j=off[1];(pos[0]+i<15&&i+pos[0]>=0)&& (pos[1] + j < 15 && j + pos[1] >= 0); i+=off[0],j+=off[1])
        {
            //if (num > 4) return true;
            if (grid[pos[0] + i, pos[1] + j] == (int)turn) 
            {
                num++;
                //Debug.Log("右边增加了");
            }
            //如果不等
            else
            {
                break;
            }
            
        }
        //向左扫，偏移量取负
        for (int i = -off[0], j = -off[1]; (pos[0] + i < 15 && i + pos[0] >= 0) && (pos[1] + j < 15 && j + pos[1] >= 0); i -= off[0], j -= off[1])
        {
            //if (num > 4) return true;
            if (grid[pos[0] + i, pos[1] + j] == (int)turn) 
            {
                num++;
                //Debug.Log("左边增加了");
            }
            else
            {
                break;
            } 
        }
        if (num > 4) { return true; }
        return false;
    }
    //检测游戏是结束
    void GameEnd()
    {
        //游戏进行状态结束        
        gameStart = false;
        string[] s = { "", "黑", "白" };
        //游戏结束，显示其父亲游戏物体，即显示该游戏物体
        winner.transform.parent.parent.gameObject.SetActive(true);
        if ((int)turn == 1)
        {
            winner.text = s[(int)turn] + "棋胜！";
        }
        else if((int)turn == 2)
        {
            winner.color = new Color32(255,255,255,255);
            winner.text = s[(int)turn] + "棋胜！";
        }
       
        Debug.Log(s[(int)turn] + "赢了");
        //print("对局已结束");
    }
    //悔棋
    public void RetractChess()
    {
        if (s.Count > 1)
        {
            //出栈
            GameObject temp = s.Pop();
            //置棋子该点为0，要注意这里不是加7.5，而是加7，因为加7.5是屏幕获得的坐标，加7是在Unity坐标系下
            grid[(int)(temp.transform.position.x + 7),(int)(temp.transform.position.y + 7)]=0;
            //销毁该颗棋子
            Destroy(temp);
            //再执行一次，销毁另一颗棋子
            //出栈
            GameObject temp2 = s.Pop();
            grid[(int)(temp2.transform.position.x + 7), (int)(temp2.transform.position.y + 7)] = 0;
            //销毁该颗棋子
            Destroy(temp2);
        }
        else if (s.Count == 1)
        {
            //出栈
            GameObject temp = s.Pop();
            grid[(int)(temp.transform.position.x + 7), (int)(temp.transform.position.y + 7)] = 0;
            //销毁该颗棋子
            Destroy(temp);
        }
    }
    // Use this for initialization
    void Start () {

        grid = new int[15, 15];//初始化棋盘，值为0
    }
	private void FixedUpdate()
    {
        timer += Time.deltaTime;//下期时，不断增加timer
    }
}
