using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
//该脚本没写好
public class Sound : MonoBehaviour {
    //音乐播放
    public AudioClip[] stings;
    public AudioSource mySource;
    void PlaySting()
    {
        //如果目前场景是第一场景或第二个场景的话，就播第一个音乐
        //获取编号
        int index = SceneManager.GetActiveScene().buildIndex;
        if (0 <= index && index <= 5)
        {
            mySource.clip = stings[0];
            mySource.Play();
        }
    }
}
