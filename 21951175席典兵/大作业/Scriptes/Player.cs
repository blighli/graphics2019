using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;//反穿透
using UnityEngine.UI;
public class Player : MonoBehaviour {


    public ChessType chessColor = ChessType.Black;
    //悔棋按钮
    Button btn;
    //是否为双人模式
    public bool isDoubleMode = false;
    //玩家实例
    static Player _instancee;
    public static Player Instancee
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
    }
    //三个AI继承自Player
    protected virtual void Start()
    {
        //获取“悔棋”按钮对象的组件Button
        btn = GameObject.Find("RetractBtn").GetComponent<Button>();
        //判断是否选择了双人模式
        if (PlayerPrefs.GetInt("Double")==10)
        {
            isDoubleMode = true;
        }
        //输出玩家的IP
        Debug.LogWarning("IP地址为："+Network.player.ipAddress);
    }
    protected virtual void FixedUpdate()
    {
        //turn的颜色和选择的颜色相同且间隔大于0.3f，才能下棋

        if(ChessBoard.Instance.turn==chessColor) PlayerChess();
        //如果选择了双人模式
        if(!isDoubleMode) ChangeBtnColor();
        
    }
    virtual public void PlayerChess()//实现动态
    {
        //鼠标左键点击且点击到的是棋盘部分
        if (Input.GetMouseButtonDown(0)&&!EventSystem.current.IsPointerOverGameObject())
        {
            //获取鼠标点击的坐标，再转换成Unity中的3D坐标
            Vector2 pos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
            ////进行四舍五入，防止总取小值
            //print((int)(pos.x + 7.5) + " " + (int)(pos.y + 7.5));
            //在该位置处下棋
            if(ChessBoard.Instance.PlayChess(new int[2] { (int)(pos.x + 7.5), (int)(pos.y + 7.5) })) 
                ChessBoard.Instance.timer = 0;//下完之后归0

        }
    }
    //激活和隐藏悔棋按钮
    protected virtual void ChangeBtnColor()
    {
        //如果玩家是Watch状态
        if (chessColor == ChessType.Watch)
        {
            return;
        }
        //如果是玩家下棋时
        if (chessColor == ChessBoard.Instance.turn)
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
}
