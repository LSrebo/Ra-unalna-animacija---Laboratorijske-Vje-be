using UnityEngine;
using System.Collections.Generic;

public class PipeSpawner : MonoBehaviour
{
    public GameObject pipePrefab;
    public ScoreManager scoreManager;
    public float spawnRate = 2f;
    public float minHeight = -2f;
    public float maxHeight = 2f;
    public float startPipeSpeed = 2f;
    public float speedIncrease = 0.1f;

    private float nextSpawnTime = 0f;
    private bool isGameOver = false;
    private List<GameObject> pipes = new List<GameObject>();

    void Update()
    {
        if (!isGameOver)
        {
            if (Time.time >= nextSpawnTime)
            {
                SpawnPipe();
                nextSpawnTime = Time.time + spawnRate;
            }
        }
    }

    void SpawnPipe()
    {
        float randomHeight = Random.Range(minHeight, maxHeight);
        Vector3 spawnPosition = new Vector3(transform.position.x, randomHeight, 0f);

        float currentPipeSpeed = startPipeSpeed + (scoreManager.GetScore() * speedIncrease);

        GameObject pipe = Instantiate(pipePrefab, spawnPosition, Quaternion.identity);
        pipes.Add(pipe);

        //pipe.GetComponent<PipeMovement>().SetSpeed(currentPipeSpeed);
    }

    public void SetGameOver(bool gameOver)
    {
        isGameOver = gameOver;

        foreach(GameObject pipe in pipes)
        {
            pipe.GetComponent<PipeMovement>().SetGameOver(true);
        }
    }

    public bool GetGameOver()
    {
        return isGameOver;
    }

    public void RemovePipeFromList(GameObject pipe)
    {
        pipes.Remove(pipe);
    }
}