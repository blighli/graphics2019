using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AILevelOne : Player {

    //棋谱,string表示棋型，float表示分数
    protected Dictionary<string, float> toScore = new Dictionary<string, float>();
    //打分表，对每一个坐标点的评分
    protected float[,] Score = new float[15,15];
    protected int oppoChessColor;
    void Awake()
    {
        oppoChessColor = (int)GameObject.Find("Player").GetComponent<Player>().chessColor;
    }
    //覆盖了父类的Start方法
    protected override void Start () {
        //此棋谱的表示的含义是落子完后形成的将形成的棋谱
        //二子棋谱
        toScore.Add("_aa", 50);
        toScore.Add("aa_", 50);
        toScore.Add("_aa_", 100);
        //三子棋谱
        toScore.Add("_aaa_", 1000);
        toScore.Add("aaa_", 500);
        toScore.Add("_aaa", 500);
        //toScore.Add("a_a_a", 500);
        //toScore.Add("_a_a_a", 500);
        //四子棋谱
        toScore.Add("_aaaa_", 10000);
        toScore.Add("_aaaa", 5000);
        toScore.Add("aaaa_", 5000);
        //toScore.Add("a_aaa", 5000);
        //五子棋谱
        toScore.Add("aaaaa", float.MaxValue);
        toScore.Add("_aaaaa", float.MaxValue);
        toScore.Add("aaaaa_",float.MaxValue);
        toScore.Add("_aaaaa_", float.MaxValue);
        //获取对方五子棋颜色
        if(chessColor!=ChessType.Watch)
                 Debug.Log(chessColor + "一级AI");
    }
    //打分函数-AI核心扫棋函数
    protected virtual void CheckOneLine(int[] pos, int[] off,int chess,bool isAttack)
    {
        string s = "a";
        //往右边扫
        //判断是否满足边界
        for (int i = off[0], j = off[1]; (pos[0] + i < 15 && i + pos[0] >= 0) && (pos[1] + j < 15 && j + pos[1] >= 0); i += off[0], j += off[1])
        {
            //同子
            if (ChessBoard.Instance.grid[pos[0] + i, pos[1] + j] == chess)
            {
                s += "a";
            }
            //如果空子
            else if (ChessBoard.Instance.grid[pos[0] + i, pos[1] + j] == 0)
            {
                s += "_";
                break;
            }
            //异子
            else
            {
                break;
            }
        }
        //向左扫，偏移量取负
        for (int i = -off[0], j = -off[1]; (pos[0] + i < 15 && i + pos[0] >= 0) && (pos[1] + j < 15 && j + pos[1] >= 0); i -= off[0], j -= off[1])
        {
            //同子
            if (ChessBoard.Instance.grid[pos[0] + i, pos[1] + j] == chess)
            {
                //往左边扫，字符串加在左边
                s = "a" + s;
            }
            //如果空子
            else if (ChessBoard.Instance.grid[pos[0] + i, pos[1] + j] == 0)
            {
                s = "_" + s;
                break;
            }
            //异子
            else
            {
                break;
            }
        }

        /*进行打分*/
        //如果该s在棋谱中存在
        //进攻比防守优先
        if (toScore.ContainsKey(s)&&isAttack)
        {
            //设置该点分数,每个点分数是累加的
            Score[pos[0], pos[1]]+= toScore[s]*1.2f;
        }else if (toScore.ContainsKey(s) && isAttack==false)
        {
            Score[pos[0], pos[1]] += toScore[s];
        }
    }
    //评分
    protected void setScore(int[] pos)
    {
        //每次评分之前要清空该点得分
        Score[pos[0], pos[1]] = 0;
        //对该点的0,45,90,135四种方位进行遍历
        //先对进攻评分
        CheckOneLine(pos, new int[2] { 1, 0 }, (int)chessColor, true);
        CheckOneLine(pos, new int[2] { 0, 1 }, (int)chessColor, true);
        CheckOneLine(pos, new int[2] { 1, 1 }, (int)chessColor, true);
        CheckOneLine(pos, new int[2] { 1, -1 }, (int)chessColor, true);
      
        //对防守评分
        CheckOneLine(pos, new int[2] { 1, 0 },oppoChessColor , false);
        CheckOneLine(pos, new int[2] { 0, 1 }, oppoChessColor, false);
        CheckOneLine(pos, new int[2] { 1, 1 }, oppoChessColor, false);
        CheckOneLine(pos, new int[2] { 1, -1 }, oppoChessColor, false);
      

    }
    //重写父类的下棋函数
    public override void PlayerChess()
    {
        //当前棋谱没有一颗棋子时，默认下在中心点---减少时间复杂度
        //print("栈中共有:" + ChessBoard.Instance.s.Count);
        if (ChessBoard.Instance.s.Count == 0)
        {
            //下棋，下到中心点
            if(ChessBoard.Instance.PlayChess(new int[2] { 7,7 }))
            {
                //Debug.Log("AI 在原始位置处开始下棋了");
                ChessBoard.Instance.timer = 0;//时间归0
            }
            //下完棋
            return;
        }
        //有棋子时
        float maxScore = 0;
        int[] maxPos = new int[2] { 0, 0 };
        for(int i = 0; i < 15; i++)
        {
            for(int j = 0; j < 15; j++)
            {
                //对该点评分
                setScore(new int[2] { i,j});
                if (Score[i,j] >= maxScore&&ChessBoard.Instance.grid[i,j]==0)//该点分数更高
                {
                    maxScore = Score[i, j];
                    maxPos[0] = i;
                    maxPos[1] = j;
                }
            }
        }
        //在最高分处下棋
        //Debug.LogWarning("x:" + maxPos[0] + "y:" + maxPos[1]);
        //Debug.LogWarning("maxScore:" + maxScore);
        if (ChessBoard.Instance.PlayChess(maxPos))
        {
            ChessBoard.Instance.timer = 0;//时间归0
        }
    }
    protected override void ChangeBtnColor()
    {
    }
}
