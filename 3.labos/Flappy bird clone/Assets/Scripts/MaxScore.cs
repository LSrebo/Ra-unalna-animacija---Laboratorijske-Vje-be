using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class MaxScore : MonoBehaviour
{
    public ScoreManager scoreManager;
    public Text currentScoreText;
    public Text maxScoreText;
    public Text youDied;
    public Button returnToMainMenu;

    private bool gameOver = false;

    private int maxScore;

    private void Start()
    {
        maxScore = PlayerPrefs.GetInt("MaxScore", 0);
        gameOver = false;
        returnToMainMenu.onClick.AddListener(ReloadScene);
    }

    private void Update()
    {
        if (gameOver)
        {
            if(scoreManager.GetScore() > maxScore)
            {
                maxScore = scoreManager.GetScore();
                PlayerPrefs.SetInt("MaxScore", maxScore);
            }
            returnToMainMenu.gameObject.SetActive(true);
            currentScoreText.text = "Score: " + scoreManager.GetScore().ToString();
            maxScoreText.text = "Max score: " + maxScore.ToString();
            currentScoreText.gameObject.SetActive(true);
            maxScoreText.gameObject.SetActive(true);
            youDied.gameObject.SetActive(true);
        }

        
    }

    public void SetGameOver(bool isGameOver)
    {
        gameOver = isGameOver;
    }

    public void ReloadScene()
    {
        int currentSceneIndex = SceneManager.GetActiveScene().buildIndex;
        SceneManager.LoadScene(currentSceneIndex);
    }
}
