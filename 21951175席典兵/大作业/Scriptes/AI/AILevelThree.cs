using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MiniMaxNode
{
    public int chess;
    public int[] pos;
    public List<MiniMaxNode> child;
    public float value;
}

public class AILevelThree : Player
{

    Dictionary<string, float> toScore = new Dictionary<string, float>();


    //protected override void ChangeBtnColor()
    //{
    //}

    protected override void Start()
    {
        toScore.Add("aa___", 100);                      //眠二
        toScore.Add("a_a__", 100);
        toScore.Add("___aa", 100);
        toScore.Add("__a_a", 100);
        toScore.Add("a__a_", 100);
        toScore.Add("_a__a", 100);
        toScore.Add("a___a", 100);


        toScore.Add("__aa__", 500);                     //活二 
        toScore.Add("_a_a_", 500);
        toScore.Add("_a__a_", 500);

        toScore.Add("_aa__", 500);
        toScore.Add("__aa_", 500);


        toScore.Add("a_a_a", 1000);
        toScore.Add("aa__a", 1000);
        toScore.Add("_aa_a", 1000);
        toScore.Add("a_aa_", 1000);
        toScore.Add("_a_aa", 1000);
        toScore.Add("aa_a_", 1000);
        toScore.Add("aaa__", 1000);                     //眠三

        toScore.Add("_aa_a_", 9000);                    //跳活三
        toScore.Add("_a_aa_", 9000);

        toScore.Add("_aaa_", 10000);                    //活三       


        toScore.Add("a_aaa", 15000);                    //冲四
        toScore.Add("aaa_a", 15000);                    //冲四
        toScore.Add("_aaaa", 15000);                    //冲四
        toScore.Add("aaaa_", 15000);                    //冲四
        toScore.Add("aa_aa", 15000);                    //冲四        


        toScore.Add("_aaaa_",float.MaxValue);          //活四

        toScore.Add("aaaaa",float.MaxValue);           //连五


        if (chessColor != ChessType.Watch)
            Debug.Log(chessColor + "3级AI");
    }

    public float CheckOneLine(int[,] grid, int[] pos, int[] offset, int chess)
    {
        float score = 0;
        bool lfirst = true, lstop = false, rstop = false;
        int AllNum = 1;
        string str = "a";
        int ri = offset[0], rj = offset[1];
        int li = -offset[0], lj = -offset[1];
        while (AllNum < 7 && (!lstop || !rstop))
        {
            if (lfirst)
            {
                //左边
                if ((pos[0] + li >= 0 && pos[0] + li < 15) &&
            pos[1] + lj >= 0 && pos[1] + lj < 15 && !lstop)
                {
                    if (grid[pos[0] + li, pos[1] + lj] == chess)
                    {
                        AllNum++;
                        str = "a" + str;

                    }
                    else if (grid[pos[0] + li, pos[1] + lj] == 0)
                    {
                        AllNum++;
                        str = "_" + str;
                        if (!rstop) lfirst = false;
                    }
                    else
                    {
                        lstop = true;
                        if (!rstop) lfirst = false;
                    }
                    li -= offset[0]; lj -= offset[1];
                }
                else
                {
                    lstop = true;
                    if (!rstop) lfirst = false;
                }
            }
            else
            {
                if ((pos[0] + ri >= 0 && pos[0] + ri < 15) &&
          pos[1] + rj >= 0 && pos[1] + rj < 15 && !lfirst && !rstop)
                {
                    if (grid[pos[0] + ri, pos[1] + rj] == chess)
                    {
                        AllNum++;
                        str += "a";

                    }
                    else if (grid[pos[0] + ri, pos[1] + rj] == 0)
                    {
                        AllNum++;
                        str += "_";
                        if (!lstop) lfirst = true;
                    }
                    else
                    {
                        rstop = true;
                        if (!lstop) lfirst = true;
                    }
                    ri += offset[0]; rj += offset[1];
                }
                else
                {
                    rstop = true;
                    if (!lstop) lfirst = true;
                }
            }
        }

        string cmpStr = "";
        foreach (var keyInfo in toScore)
        {
            if (str.Contains(keyInfo.Key))
            {
                if (cmpStr != "")
                {
                    if (toScore[keyInfo.Key] > toScore[cmpStr])
                    {
                        cmpStr = keyInfo.Key;
                    }
                }
                else
                {
                    cmpStr = keyInfo.Key;
                }
            }
        }

        if (cmpStr != "")
        {
            score += toScore[cmpStr];
        }
        return score;
    }

    public float GetScore(int[,] grid, int[] pos)
    {
        float score = 0;

        score += CheckOneLine(grid, pos, new int[2] { 1, 0 }, 1);
        score += CheckOneLine(grid, pos, new int[2] { 1, 1 }, 1);
        score += CheckOneLine(grid, pos, new int[2] { 1, -1 }, 1);
        score += CheckOneLine(grid, pos, new int[2] { 0, 1 }, 1);

        score += CheckOneLine(grid, pos, new int[2] { 1, 0 }, 2);
        score += CheckOneLine(grid, pos, new int[2] { 1, 1 }, 2);
        score += CheckOneLine(grid, pos, new int[2] { 1, -1 }, 2);
        score += CheckOneLine(grid, pos, new int[2] { 0, 1 }, 2);

        return score;
    }

    public override void PlayerChess()
    {
        if (ChessBoard.Instance.s.Count == 0)
        {
            if (ChessBoard.Instance.PlayChess(new int[2] { 7, 7 }))
                ChessBoard.Instance.timer = 0;
            return;
        }

        MiniMaxNode node = null;
        foreach (var item in GetList(ChessBoard.Instance.grid, (int)chessColor, true))
        {
            CreateTree(item, (int[,])ChessBoard.Instance.grid.Clone(), 4, false);
            float a = float.MinValue;
            float b = float.MaxValue;
            item.value += AlphaBeta(item, 4, false, a, b);
            if (node != null)
            {//挑选最大的下旗点
                if (node.value < item.value)
                    node = item;
            }
            else
            {
                node = item;
            }
        }
        print("AI分数"+node.value);
        ChessBoard.Instance.PlayChess(node.pos);
    }

    //返回节点 极大极小
    List<MiniMaxNode> GetList(int[,] grid, int chess, bool mySelf)
    {
        List<MiniMaxNode> nodeList = new List<MiniMaxNode>();
        MiniMaxNode node;
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                int[] pos = new int[2] { i, j };
                if (grid[pos[0], pos[1]] != 0) continue;

                node = new MiniMaxNode();
                node.pos = pos;
                node.chess = chess;
                if (mySelf)
                    node.value = GetScore(grid, pos);
                else
                    node.value = -GetScore(grid, pos);
                if (nodeList.Count <5)
                {
                    nodeList.Add(node);
                }
                else
                {
                    foreach (var item in nodeList)
                    {
                        if (mySelf)//极大点
                        {
                            if (node.value > item.value)
                            {
                                nodeList.Remove(item);
                                nodeList.Add(node);
                                break;
                            }
                        }
                        else//极小点
                        {
                            if (node.value < item.value)
                            {
                                nodeList.Remove(item);
                                nodeList.Add(node);
                                break;
                            }
                        }
                    }
                }
            }
        }

        return nodeList;
    }

    //创建树
    public void CreateTree(MiniMaxNode node, int[,] grid, int deep, bool mySelf)
    {
        if (deep == 0 || node.value == float.MaxValue)
        {
            return;
        }
        grid[node.pos[0], node.pos[1]] = node.chess;
        node.child = GetList(grid, node.chess, !mySelf);
        foreach (var item in node.child)
        {
            CreateTree(item, (int[,])grid.Clone(), deep - 1, !mySelf);
        }
    }


    public float AlphaBeta(MiniMaxNode node, int deep, bool mySelf, float alpha, float beta)
    {

        if (deep == 0 || node.value == float.MaxValue || node.value == float.MinValue)
        {
            return node.value;
        }

        if (mySelf)
        {
            foreach (var child in node.child)
            {
                alpha = Mathf.Max(alpha, AlphaBeta(child, deep - 1, !mySelf, alpha, beta));

                //alpha剪枝

                if (alpha >= beta)
                {
                    return alpha;
                }

            }
            return alpha;
        }
        else
        {
            foreach (var child in node.child)
            {
                beta = Mathf.Min(beta, AlphaBeta(child, deep - 1, !mySelf, alpha, beta));

                //beta剪枝
                if (alpha >= beta)
                {
                    return beta;
                }

            }
            return beta;
        }
    }
    protected override void ChangeBtnColor()
    {
    }

}
