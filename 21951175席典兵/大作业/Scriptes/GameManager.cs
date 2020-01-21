using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour {
    //player数组，保存将有的所有Player
    public List<Player> playList = new List<Player>();
  
    //设置初始的玩家
    public void Awake()
    {
        SetChange();
        //该值的作用在于每次使该值获得刷新，防止其一直为10，因此采取的解决思路为是该值在玩家点击“返回”按钮时进行刷新
        //PlayerPrefs.SetInt("Double", 1);
    }
    void SetChange()
    {
        int player1 = PlayerPrefs.GetInt("Player1");
        int player2 = PlayerPrefs.GetInt("Player2");
        for (int i = 0; i < playList.Count; i++)
        {
            //1号玩家设置为黑色
            if (player1 == i)
            {
                playList[i].chessColor = ChessType.Black;
            }
            //2号玩家设置为白色
            else if (player2 == i)
            {
                playList[i].chessColor = ChessType.White;
            }
            //其他玩家设置为观战
            else
            {
                playList[i].chessColor = ChessType.Watch;
            }

        }
    }
    //设置使用哪号玩家
    public void SetPlayer1(int i)
    {
        //设置1号玩家,通过注册表方式，进行存写
        PlayerPrefs.SetInt("Player1", i);
    }
    public void SetPlayer2(int i)
    {
        //设置2号玩家
        PlayerPrefs.SetInt("Player2", i);
    }
    //设置开始游戏场景
    public void PlayGame()
    {
        SceneManager.LoadScene(1);
    }
    //设置联网模式
    public void PlayNetGame()
    {
        SceneManager.LoadScene(2);
    }
    public void ChangeChessColor()
    {
        //因为1号玩家是先走的，默认黑棋，更换先手其实就是让谁成为1号玩家，谁为2号
        for (int i = 0; i < playList.Count; i++)
        {
            //之前为黑色，则将其设置为2号玩家
            if (playList[i].chessColor == ChessType.Black)
            {
                SetPlayer2(i);
            }
            //2号玩家设置为白色
            else if (playList[i].chessColor==ChessType.White)
            {
                SetPlayer1(i);
            }
        }
        //重新设置选手顺序
        SetChange();
        //重新加载1号游戏场景
        SceneManager.LoadScene(1);
    }
    public void DoubleMode()
    {
        //Double=10,表示激活了双人模式
        PlayerPrefs.SetInt("Double", 10);
    }
}
