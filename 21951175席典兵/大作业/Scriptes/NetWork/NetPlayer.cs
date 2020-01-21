using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;//反穿透
using UnityEngine.UI;
using UnityEngine.Networking;

public class NetPlayer : NetworkBehaviour {

    //将玩家棋号设置为同步
    [SyncVar]
    public ChessType chessColor = ChessType.Black;
    //悔棋按钮
    Button btn;
    //是否为双人模式
    public bool isDoubleMode = false;
    //玩家实例
    static NetPlayer _instancee;
    public static NetPlayer Instancee
    {
        get
        {
            return _instancee;
        }
    }
    void Awake()
    {
        if (Instancee == null)
        {
            _instancee = this;
        }
        //获取按钮组件,先获取该对象，再获取其组件
        //GameObject.Find("RetractBtn").GetComponent<Button>();下面等价
        btn = GameObject.Find("RetractBtn").transform.GetComponent<Button>();
    }
    //三个AI继承自Player
    protected virtual void Start()
    {
        //isLocalPlayer是NetWorking中自带的
        if (isLocalPlayer)
        {
            //调用服务器端的生成玩家函数
            CmdSetPlayer();
            //当前玩家为观察着
            if (chessColor == ChessType.Watch) return;
            
            //向按钮组件添加监听事件
            btn.onClick.AddListener(ClickRetract);
        }
        //输出玩家的IP
        Debug.LogWarning("IP地址为：" + Network.player.ipAddress);
    }
    void FixedUpdate()
    {
        //turn的颜色和选择的颜色相同且间隔大于0.3f，才能下棋
        //判断是不是我方客户端玩家，是才能下棋
        //且人数大于1才能下棋
        if(NetChessBoard.Instance.turn==chessColor && isLocalPlayer&&NetChessBoard.Instance.PlayerNumber>1) PlayerChess();
        ////如果选择了双人模式
        //if(!isDoubleMode) ChangeBtnColor();
        //如果当前下棋者非观战玩家且游戏结束，则输出结算画面
        if (NetChessBoard.Instance.turn != ChessType.Watch && NetChessBoard.Instance.gameStart == false)
        {
            NetChessBoard.Instance.GameEnd(NetChessBoard.Instance.win);
        }
        //如果当前下棋的玩家不是观战者，且不是本地玩家，且游戏仍然在运行
        if (NetChessBoard.Instance.turn!=ChessType.Watch&&isLocalPlayer && NetChessBoard.Instance.gameStart == true)
        {
            ChangeBtnColor();
        }
        
    }
    void PlayerChess()//实现动态
    {
        //鼠标左键点击且点击到的是棋盘部分
        if (Input.GetMouseButtonDown(0)&&!EventSystem.current.IsPointerOverGameObject())
        {
            //获取鼠标点击的坐标，再转换成Unity中的3D坐标
            Vector2 pos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
            ////进行四舍五入，防止总取小值
            //print((int)(pos.x + 7.5) + " " + (int)(pos.y + 7.5));
            //在该位置处下棋
            //if(NetChessBoard.Instance.PlayChess(new int[2] { (int)(pos.x + 7.5), (int)(pos.y + 7.5) })) 
            //    NetChessBoard.Instance.timer = 0;//下完之后归0
            //调用服务器程序
            CmdChess(pos);
        }
    }
    [Command]
    public void CmdChess(Vector2 pos)
    {
        if (NetChessBoard.Instance.PlayChess(new int[2] { (int)(pos.x + 7.5), (int)(pos.y + 7.5) }))
        {
            NetChessBoard.Instance.timer = 0;//下完之后归0
        }
    }

    //激活和隐藏悔棋按钮
    void ChangeBtnColor()
    {
        //如果是玩家下棋时
        if (chessColor == NetChessBoard.Instance.turn)
        {
            //设置按钮可点击
            btn.interactable = true;
        }
        //设置按钮不可点击
        else
        {
            btn.interactable = false;
        }
    }
    //服务器端函数，由服务器生成玩家
    [Command]
    public void CmdSetPlayer()
    {
        NetChessBoard.Instance.PlayerNumber++;
        if (NetChessBoard.Instance.PlayerNumber == 1)
        {
            chessColor = ChessType.Black;
        }
        else if (NetChessBoard.Instance.PlayerNumber == 2)
        {
            chessColor = ChessType.White;
        }
        else
        {
            chessColor = ChessType.Watch;
        }
    }
    //鼠标点击悔棋函数
    void ClickRetract()
    {
        CmdRetract();
    }
    //服务器端悔棋函数
    [Command]
    public void CmdRetract()
    {
        NetChessBoard.Instance.RetractChess();
    }
}
