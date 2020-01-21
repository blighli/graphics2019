using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using System.Text;
using System.IO;
using System.Collections.Generic;

[AddComponentMenu("Game/GameManager")]
public class GameManager : MonoBehaviour {

     public static GameManager Instance = null;

    Button button_d;
    Button button_s;
    Button button_p;
    Button button_c;
    Light light;


    void Start () {
 
        Time.timeScale = 1;

        Instance = this;

        GameObject lightObj = GameObject.Find("Light");
        light = lightObj.GetComponent<Light>();

        GameObject uicanvas = GameObject.Find("Canvas");
        foreach (Transform t in uicanvas.transform.GetComponentsInChildren<Transform>())
        {

  
            if (t.name.CompareTo("Directional") == 0)
            {
                button_d = t.GetComponent<Button>();
                button_d.onClick.AddListener(delegate ()
                {
                    light.type= UnityEngine.LightType.Directional;  
                });
            }
            else if (t.name.CompareTo("Point") == 0)
            {
                button_s = t.GetComponent<Button>();
                button_s.onClick.AddListener(delegate ()
                {
                    light.type= UnityEngine.LightType.Point;  
                });
            }
            else if (t.name.CompareTo("Spot") == 0)
            {
                button_p = t.GetComponent<Button>();
                button_p.onClick.AddListener(delegate ()
                {
                    light.type= UnityEngine.LightType.Spot;
                });  

            }
            else if (t.name.CompareTo("Color") == 0)
            {
                button_c = t.GetComponent<Button>();
                button_c.onClick.AddListener(delegate ()
                {

                    light.color = new Color(Random.value, Random.value, Random.value);
                });  

            }
        }
       
      

    }

    void Update()
    {



    }

    

    
}