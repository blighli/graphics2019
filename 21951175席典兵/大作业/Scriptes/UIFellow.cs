using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class UIFellow : MonoBehaviour {

	// Update is called once per frame
	void Update () {
        if (ChessBoard.Instance.s.Count > 0)
        {
            //将当前栈顶元素的坐标置给追踪器
            transform.position = ChessBoard.Instance.s.Peek().transform.position;
        }
	}
    //重玩加载场景1
    public void OnRelayBtn()
    {
        SceneManager.LoadScene(1);
    }
    //返回加载场景0
    public void ReturnBtn()
    {
        SceneManager.LoadScene(0);
        //该值的作用在于每次使该值获得刷新，防止其一直为10，因此采取的解决思路为是该值在玩家点击“返回”按钮时进行刷新
        PlayerPrefs.SetInt("Double", 1);
    }
}
