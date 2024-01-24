using Unity.VisualScripting;
using UnityEngine;

public class BirdController : MonoBehaviour
{
    public float jumpForce = 5f;
    public float rotationSpeed = 2f;
    public PipeSpawner pipeSpawner;
    public ScoreManager scoreManager;
    public MaxScore maxScore;
    public GameObject coinPickupSound;
    public GameObject birdFlapSound;
    public GameObject hitSound;


    private Rigidbody2D rb;
    private bool isFalling = false;
    private bool isGameOver = false;
    private bool hitSoundPlayed = false;
    private bool gameStarted = false;

    void Start()
    {
        rb = GetComponent<Rigidbody2D>();
    }

    void Update()
    {
        if (isGameOver)
        {
            return;
        }

        if (gameStarted)
        {
            if (Input.GetKeyDown(KeyCode.Space))
            {
                Jump();

                AudioSource birdFlapAudioSource = birdFlapSound.GetComponent<AudioSource>();
                if (birdFlapAudioSource != null && birdFlapAudioSource.clip != null)
                {
                    birdFlapAudioSource.Play();
                }
            }
            isFalling = rb.velocity.y < 0;

            RotateBird();

        }

        
    }

    void Jump()
    {
        rb.velocity = new Vector2(rb.velocity.x, 0f);
        rb.AddForce(Vector2.up * jumpForce, ForceMode2D.Impulse);
        rb.velocity = new Vector2(0f, rb.velocity.y);
    }

    void RotateBird()
    {
        float targetRotationAngle;

        if (isFalling)
        {
            targetRotationAngle = Mathf.Lerp(0f, -90f, -rb.velocity.y / 5f);
        }
        else
        {
            targetRotationAngle = Mathf.Lerp(0f, 40f, rb.velocity.y / 5f);
        }

        float rotationSpeed = 8f;
        float currentRotation = transform.rotation.eulerAngles.z;
        float newRotation = Mathf.LerpAngle(currentRotation, targetRotationAngle, rotationSpeed * Time.deltaTime);

        transform.rotation = Quaternion.Euler(0f, 0f, newRotation);
    }

    public void SetGameStarted(bool isGameStarted)
    {
        gameStarted = isGameStarted;
    }
    void OnCollisionEnter2D(Collision2D collision)
    {
        if(collision.gameObject.CompareTag("Pipe") || collision.gameObject.CompareTag("Ground"))
        {
            isFalling = true;
            rb.velocity = new Vector2(0f, -5f);
            rb.gravityScale = 2f;
            rb.constraints = RigidbodyConstraints2D.FreezePositionX;
            isGameOver = true;

            pipeSpawner.SetGameOver(true);
            maxScore.SetGameOver(true);

            if (!hitSoundPlayed)
            {
                AudioSource hitAudioSource = hitSound.GetComponent<AudioSource>();
                if (hitAudioSource != null && hitAudioSource.clip != null)
                {
                    hitAudioSource.Play();
                }

                hitSoundPlayed = true;
            }
        }
    }

    void OnTriggerEnter2D(Collider2D collider)
    {
        if (collider.CompareTag("Score") && !hitSoundPlayed)
        {
            scoreManager.IncreaseScore();

            AudioSource coinAudioSource = coinPickupSound.GetComponent<AudioSource>();
            if(coinAudioSource != null && coinAudioSource.clip != null)
            {
                coinAudioSource.Play();
            }
        }
    }
}