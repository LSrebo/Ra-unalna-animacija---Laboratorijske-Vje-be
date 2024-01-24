using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class MainMenuManager : MonoBehaviour
{

    public Button playGame;
    public Button quitGame;
    public Button returnToMainMenu;
    public Text gameName;
    public Text score;
    public Text waitForSpace;
    public GameObject player;
    public Text currentScoreText;
    public Text maxScoreText;
    public Text youDied;
    public BirdController birdController;
    public StartGame startGame;

    public MonoBehaviour[] scriptsToDisable;

    private void Start()
    {
        playGame.onClick.AddListener(StartGame);
        quitGame.onClick.AddListener(QuitGame);
        score.gameObject.SetActive(false);
        waitForSpace.gameObject.SetActive(false);
        currentScoreText.gameObject.SetActive(false);
        maxScoreText.gameObject.SetActive(false);
        youDied.gameObject.SetActive(false);
        returnToMainMenu.gameObject.SetActive(false);
        player.gameObject.GetComponent<Rigidbody2D>().isKinematic = true;
        DisableScripts();
    }
    public void StartGame()
    {
        playGame.gameObject.SetActive(false);
        quitGame.gameObject.SetActive(false);
        gameName.gameObject.SetActive(false);
        score.gameObject.SetActive(true);
        waitForSpace.gameObject.SetActive(true);
        birdController.SetGameStarted(true);
        startGame.SetGameStarted(true);
    }

    public void QuitGame()
    {
        Application.Quit();
    }

    void DisableScripts()
    {
        foreach (var script in scriptsToDisable)
        {
            if (script != null)
            {
                script.enabled = false;
            }
        }
    }
}

