using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AILevelTwo : AILevelOne
{


       protected override void Start()
    {
        toScore.Add("a_",50);
        toScore.Add("_a", 50);
        toScore.Add("_a_", 80);
        toScore.Add("a", 50);
        toScore.Add("aa___", 100);                      //眠二
        toScore.Add("a_a__", 100);
        toScore.Add("___aa", 100);
        toScore.Add("__a_a", 100);
        toScore.Add("a__a_", 100);
        toScore.Add("_a__a", 100);
        toScore.Add("a___a", 100);


        toScore.Add("__aa__", 500);                     //活二 "_aa___"
        toScore.Add("_a_a_", 500);
        toScore.Add("_a__a_", 500);

        toScore.Add("_aa__", 500);
        toScore.Add("__aa_", 500);


        toScore.Add("a_a_a", 1000);                     // bool lfirst = true, lstop,rstop = false  int AllNum = 1
        toScore.Add("aa__a", 1000);
        toScore.Add("_aa_a", 1000);
        toScore.Add("a_aa_", 1000);
        toScore.Add("_a_aa", 1000);
        toScore.Add("aa_a_", 1000);
        toScore.Add("aaa__", 1000);                     //眠三

        toScore.Add("_aa_a_", 7000);                    //跳活三
        toScore.Add("_a_aa_", 7000);

        toScore.Add("_aaa_", 10000);                    //活三       


        toScore.Add("a_aaa", 15000);                    //冲四
        toScore.Add("aaa_a", 15000);                    //冲四
        toScore.Add("_aaaa", 15000);                    //冲四
        toScore.Add("aaaa_", 15000);                    //冲四
        toScore.Add("aa_aa", 15000);                    //冲四        


        toScore.Add("_aaaa_", 1000000);                 //活四

        toScore.Add("aaaaa", float.MaxValue);           //连五
        if (chessColor != ChessType.Watch)
            Debug.Log(chessColor + "二级AI");
    }

    protected override void CheckOneLine(int[] pos, int[] offset, int chess,bool isAttack)
    {
        bool lfirst = true, lstop = false, rstop = false;
        int AllNum = 1;
        string str = "a";
        int ri = offset[0], rj = offset[1];
        int li = -offset[0], lj = -offset[1];
        //循环
        //字符串数小于7且（左边无障碍或右边无障碍）
        while (AllNum < 7 && (!lstop || !rstop))
        {
            //向左边走
            if (lfirst)
            {
                //没有越界，且可以向左边走
                if ((pos[0] + li >= 0 && pos[0] + li < 15) &&
            pos[1] + lj >= 0 && pos[1] + lj < 15 && !lstop)
                {
                    //同子
                    if (ChessBoard.Instance.grid[pos[0] + li, pos[1] + lj] == chess)
                    {
                        AllNum++;
                        str = "a" + str;

                    }
                    //无子
                    else if (ChessBoard.Instance.grid[pos[0] + li, pos[1] + lj] == 0)
                    {
                        AllNum++;
                        str = "_" + str;
                        //右边没有停止，向右边走
                        if (!rstop) lfirst = false;
                    }
                    //异子
                    else
                    {
                        //无法再向左边走
                        lstop = true;
                        //开始向右边走
                        if (!rstop) lfirst = false;
                    }
                    li -= offset[0]; lj -= offset[1];
                }
                //碰到墙壁
                else
                {
                    //无法再向左边走
                    lstop = true;
                    //开始向右边走
                    if (!rstop) lfirst = false;
                }
            }
            //向右边走
            else
            {
                //没有越界，且可以往右边走
                if ((pos[0] + ri >= 0 && pos[0] + ri < 15) &&
          pos[1] + rj >= 0 && pos[1] + rj < 15 && !lfirst && !rstop)
                {
                    //同子
                    if (ChessBoard.Instance.grid[pos[0] + ri, pos[1] + rj] == chess)
                    {
                        AllNum++;
                        str += "a";

                    }
                    //无子
                    else if (ChessBoard.Instance.grid[pos[0] + ri, pos[1] + rj] == 0)
                    {
                        AllNum++;
                        str += "_";
                        //左边可以走，向左边走
                        if (!lstop) lfirst = true;
                    }
                    //异子
                    else
                    {
                        //右边无法继续走
                        rstop = true;
                        //左边可以走，向左边走，
                        if (!lstop) lfirst = true;
                    }
                    ri += offset[0]; rj += offset[1];
                }
                //碰到墙壁
                else
                {
                    //右边无法继续走
                    rstop = true;
                    //左边可以继续走,向左边走
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
            if (isAttack)
            {
                Score[pos[0], pos[1]] += 1.3f*toScore[cmpStr];
            }
            else
            {
                Score[pos[0], pos[1]] +=toScore[cmpStr];
            }
            
        }
    }
    protected override void ChangeBtnColor()
    {
    }
}
