using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WizardMovement : MonoBehaviour
{
    private Vector3 direction;
    public float gravity = -9.8f;
    public float strenght = 5f;

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Space))
        {
            direction = Vector3.up * strenght;
        }
        if(Input.touchCount > 0)
        {
            Touch touch = Input.GetTouch(0);

            if(touch.phase == TouchPhase.Began)
            {
                direction = Vector3.up * strenght;
            }
        }

        direction.y += gravity * Time.deltaTime;

        transform.position += direction * Time.deltaTime;

    }
}
