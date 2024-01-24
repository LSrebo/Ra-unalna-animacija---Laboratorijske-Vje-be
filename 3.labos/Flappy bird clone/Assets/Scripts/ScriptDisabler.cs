using UnityEngine;

public class ScriptDisabler : MonoBehaviour
{
    public MonoBehaviour[] scriptsToDisable;

    private void Start()
    {
        DisableScripts();
    }

    void DisableScripts()
    {
        foreach(var script in scriptsToDisable)
        {
            if(script != null)
            {
                script.enabled = false;
            }
        }
    }
}
