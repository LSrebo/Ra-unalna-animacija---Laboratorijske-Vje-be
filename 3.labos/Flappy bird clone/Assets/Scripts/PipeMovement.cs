using UnityEngine;

public class PipeMovement : MonoBehaviour
{
    public float speed = 2f;

    private bool isGameOver = false;
    private PipeSpawner pipeSpawner;

    void Start()
    {
        pipeSpawner = GameObject.FindObjectOfType<PipeSpawner>();
    }

    void Update()
    {
        if (!isGameOver)
        {
            transform.Translate(Vector3.left * speed * Time.deltaTime);

            if (transform.position.x < -15f)
            {
                pipeSpawner.RemovePipeFromList(gameObject);
                Destroy(gameObject);
            }
        }
    }

    public void SetGameOver(bool gameOver)
    {
        isGameOver = gameOver;
    }

    public void SetSpeed(float newSpeed)
    {
        speed = newSpeed;
    }
}