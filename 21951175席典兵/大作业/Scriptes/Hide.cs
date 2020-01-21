using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Hide : MonoBehaviour {

    public void hide()
    {

        //隐藏对象————刚开始设置完但没用啊，原因在于，由于公有了该变量，所以该变量值一切以Unity所给值为准，因此在Unity上取消勾选即可
        gameObject.SetActive(false);
        //设置开始动画结束后才能下棋
        ChessBoard.Instance.gameStart = true;
    }
}
