from __future__ import print_function
import requests
import json

#-------------Controls the Bulb ------------------------------

token = "c273f5022a5eb0866c6d93deed69d973b7cfcb92d85c586764dd74f2f64a8bb1"

headers = {
    "Authorization": "Bearer %s" % token,
}

bulb_color = "green"
ipriority = "P5"

def control_bulb(priority):
    global bulb_color
    global ipriority
    ipriority = priority
    if ipriority == "major incident":
        bulb_color = "red"
    elif ipriority == "p1" or ipriority == "priority 1":
        bulb_color = "orange"
    elif ipriority == "p2" or ipriority == "priority 2":
        bulb_color = "yellow"
    else:
        bulb_color = "green"
    print("bulb state: ", "on", "bulb color ", bulb_color)
    payload = {
      "states": [
        {
            "selector" : "all",
            "brightness" : 0.5
        },
      ],
      "defaults": {
        "power": "on",
        "color": bulb_color
        }
    }

    response = requests.put('https://api.lifx.com/v1/lights/states', data=json.dumps(payload), headers=headers)
    print(response.content)



# --------------- Helpers that build all of the responses ----------------------

def build_speechlet_response(title, output, reprompt_text, should_end_session):
    return {
        'outputSpeech': {
            'type': 'PlainText',
            'text': output
        },
        'card': {
            'type': 'Simple',
            'title': "SessionSpeechlet - " + title,
            'content': "SessionSpeechlet - " + output
        },
        'reprompt': {
            'outputSpeech': {
                'type': 'PlainText',
                'text': reprompt_text
            }
        },
        'shouldEndSession': should_end_session
    }


def build_response(session_attributes, speechlet_response):
    return {
        'version': '1.0',
        'sessionAttributes': session_attributes,
        'response': speechlet_response
    }


# --------------- Functions that control the skill's behavior ------------------

def get_welcome_response():
    """ If we wanted to initialize the session to have some attributes we could
    add those here
    """

    session_attributes = {}
    card_title = "Welcome"
    speech_output = "Welcome to LiFX Alert. " \
                    "Please tell me the incident by saying, " \
                    "It is a P1 incident"
    # If the user either does not reply to the welcome message or says something
    # that is not understood, they will be prompted again with this text.
    reprompt_text = "Please tell me the incident by saying, " \
                    "It is a P1 incident"
    should_end_session = False
    return build_response(session_attributes, build_speechlet_response(
        card_title, speech_output, reprompt_text, should_end_session))


def handle_session_end_request():
    card_title = "Session Ended"
    speech_output = "Thank you for trying LiFX Alert. " \
                    "Have a nice day! "
    # Setting this to true ends the session and exits the skill.
    should_end_session = True
    return build_response({}, build_speechlet_response(
        card_title, speech_output, None, should_end_session))


def create_priority_attributes(priority):
    return {"Priority": priority}


def set_color_in_session(intent,session):
    """ Sets the color in the session and prepares the speech to reply to the
    user.
    """

    card_title = intent['name']
    session_attributes = {}
    should_end_session = False

    if 'Priority' in intent['slots']:
        priority = intent['slots']['Priority']['value']
        control_bulb(priority)
        session_attributes = create_priority_attributes(priority)
        speech_output = "I now know the incident is " + \
                        priority
    else:
        speech_output = "I'm not sure what incident that is. " \
                        "Please try again."
        reprompt_text = "I'm not sure what incident that is. " \
                        "You can tell me the incident by saying, " \
                        "P1"
    return build_response(session_attributes, build_speechlet_response(
        card_title, speech_output, reprompt_text, should_end_session))




# --------------- Events ------------------



def on_intent(intent_request, session):
    """ Called when the user specifies an intent for this skill """


    intent = intent_request['intent']
    intent_name = intent_request['intent']['name']

    # Dispatch to your skill's intent handlers
    if intent_name == "ChangeColorPriority":
        return set_color_in_session(intent,session)
    elif intent_name == "AMAZON.HelpIntent":
        return get_welcome_response()
    elif intent_name == "AMAZON.CancelIntent" or intent_name == "AMAZON.StopIntent":
        return handle_session_end_request()
    else:
        raise ValueError("Invalid intent")





# --------------- Main handler ------------------

def lambda_handler(event, context):
    """ Route the incoming request based on type (LaunchRequest, IntentRequest,
    etc.) The JSON body of the request is provided in the event parameter.
    """
    #print("event.session.application.applicationId=" + event['session']['application']['applicationId'])

    """
    Uncomment this if statement and populate with your skill's application ID to
    prevent someone else from configuring a skill that sends requests to this
    function.
    """
    # if (event['session']['application']['applicationId'] !=
    #         "amzn1.echo-sdk-ams.app.[unique-value-here]"):
    #     raise ValueError("Invalid Application ID")

    if event['session']['new'] or event['request']['type'] == "LaunchRequest":
        return get_welcome_response();
    elif event['request']['type'] == "IntentRequest":
        return on_intent(event['request'], event['session'])
    elif event['request']['type'] == "SessionEndedRequest":
        return handle_session_end_request()






