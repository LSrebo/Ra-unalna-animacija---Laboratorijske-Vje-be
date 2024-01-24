using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class StartGame : MonoBehaviour
{

    public MonoBehaviour[] scriptsToEnable;
    public GameObject player;
    public Text waitForSpace;

    private bool gameStarted = false;

    private void Start()
    {

    }

    private void Update()
    {
        if (gameStarted)
        {
            if (Input.GetKeyDown(KeyCode.Space))
            {
                waitForSpace.gameObject.SetActive(false);
                player.gameObject.GetComponent<Rigidbody2D>().isKinematic = false;
                foreach (var script in scriptsToEnable)
                {
                    if (script != null)
                    {
                        script.enabled = true;
                    }
                }
            }
        }
    }

    public void SetGameStarted(bool isGameStarted)
    {
        gameStarted = isGameStarted;
    }
}
