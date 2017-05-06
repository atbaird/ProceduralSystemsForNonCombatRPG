
//-------------------------//
Table of Contents (TOC)
//~Controls
//~GameSettings
//~SpriteResources
//~SpriteAnimations
//~TileDefinitions
//~Agent Generators
//~Hair Generators
//~Item Generators (Clothing Definitions Only)
//~Name Generators
//~NPCJobs
//~NPCAttributes
//~Agent Stats
//~Quests
//~Sound
//~SpeechBubbles
//~DialogueSegments
//~DialogueGroupings
//~MapData
//~MapFiles
//~How to make new Dialogue Requirements
//~How to make new Quest Requirements
//~How to make new Dialogue Triggers
//~How to make new Feature Behaviors
//~How to make new NPC Behaviors
//~How to make new Quest Triggers
//-------------------------//

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//~Controls
	WASD - Movement
	SPACE - Interact
	~ - If console is enabled; open/close console
	F1 - If Developer Debug is enabled; activate/deactivate developer debug.


//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//~GameSettings
The game settings xml allows the user to change:
	* The starting map.
	* The default shader program
	* What agent generator the player uses as well as the player's starting location (via TILE_COORDS (an int vector 2)) on the map.
	* The default clothing generator to use should an agent have no clothes defined for it to spawn.
	* What Font to use.
	* Where the containing folders for each data driven type is.
	* How many of the data driven types to preallocate for.
	* How many of the data driven types to load per frame.

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//~SpriteResources

Declaring a sprite resource is straight forward: each sprite resource needs a name to be referenced by, a file location for the reference image,
and the texture coordinates of the sprite on that image.

<SpriteResources>
  <SpriteResource resourceName='SpeechBubble'
                  textureLocation='Data/Images/SpeechBubble.png'
                  spriteBounds='0, 512, 512, 0'/>
</SpriteResources>

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//~SpriteAnimations

Sprite animations use sprite resources in order to declare each frame of the animation.
They take a name for the sprite animation, time intervals for each frame (in seconds), and a mode for how to handle when the animation ends (clamp by default).
modes: Clamp, Loop, PingPong

<SpriteAnimations>
  <SpriteAnimation spriteAnimationName='human_base0_walkForward' 
                   defaultIntervalBetweenFrames='0.125' 
                   animationMode='loop' 
                   reverseDirection='false'>
    <Frame resourceName='human_baseForward1'/>
    <Frame resourceName='human_baseForward2'/>
    <Frame resourceName='human_baseForward1'/>
    <Frame resourceName='human_baseForward0'/>
  </SpriteAnimation>
</SpriteAnimations>

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//~TileDefinitions
Tile Definitions have an associated name, a RGB color (MapCode), a series of 4 sprite resources (bottom, default, top1, top2 layers),
and a declaration of whether the tile is solid or not.  It is also possible to declare one set of alternative resources for each layer, as well as a percent
chance (0 to 1) for the alternative resources to be used instead.
Tile definitions can also define special cases, where in they test their neighbors in the north, east, south, and west directions for their tile types.
If the expected case finds all of the requirements to be met, then it can change tiles in any given direction.  This includes changing out sprite resources on
any layer, as well as changing whether the tile is solid or not.


<TileDefinitions>
  <!-- Basic tile definition example -->
  <TileDefinition name='grass1_1' resourceName='grassTile1_1' 
                  altResourceName='grassTile1_1_alt'
                  altResourceChance='0.5f'
                  MapCode='0,255,0' solid='false' liquid='false'/>

  <!-- Special Case example -->
  <TileDefinition name='Lake' 
                  resourceName='waterTile1_1' 
                  MapCode='0,0,255' 
                  solid='true' liquid='true'>
    
    <SpecialCase IfSame='North==0,South>0,East>0,West>0'
                 IfNeighborTypeIs='type=grass1_1,North>0'>
      <!-- N -->
      <ToSet debugName='lake_northGrass'
        TileCoords='0,0' resourceName='lake_northGrass'/>
    </SpecialCase>
    <!-- ... -->
  </TileDefinition>
</TileDefinitions>

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//~Agent Generators
TOC Agent Generators:
	//! Basic Agent Generator XML example
	//! Inheriting Agent Generator XML example

An agent is any character within the game.
Various bits of information will need to be defined on an agent, but it also can inherit a bunch of its information from another agent generator if need be.

Agent Generators Define:
	* a unique name for the agent genreator
	* movement speed
	* what name set to use, what percentage of it's spawns are what gender.
	* What hair to use for either gender or both. (procedurally generated based off gender and the lists)
	* What clothing it is allowed to select (procedurally generated based on the list)
	* What skin color the agent is allowed to be 
		- (integer values 0-6 are valid for the value; see "SpritePassthroughWithColorHandling.frag" in the "Data/Shaders" folder)
		- Which skin color is selected is procedurally generated based off the choices.
	* What NPCJobs its allowed to select from (this is procedurally generated from a compiled list of jobs that the npc meets the requirements of)
	* NPCBehaviors (what it does each frame), supported behaviors are:
		- LookAround (look about randomly and stand in place)
		- Talking (look towards the player while talking to them)
		- Wander (similar to look around, but constantly walking forward).
	* NPCAttributes
		-> what NPCAttributes to pick, and what values to black and white list.
	* The dialogue segments to pick from.
	* What animations to use per each direction
	* What stat ranges to use that are different from the common set.

//---------------------------------------
//! Basic Agent Generator XML example
//---------------------------------------
<AgentGenerators>
  <AgentGenerator
    Name='Villager' 
    DefaultImage='human_base0'
    MovementSpeed='2.f'
    NameSet='VillagerNameSet'
    PercentageMale='0.5f'>
    <Hair>
      <Both>

      </Both>

      <Male>
        <male_Townsfolk_hair1/>
        <male_Townsfolk_hair2/>
      </Male>

      <Female>
        <female_Townsfolk_hair1/>
        <female_Townsfolk_hair2/>
      </Female>
    </Hair>

    <Clothing>
      <Clothing name='Townsfolk 1 clothes'/>
      <Clothing name='Townsfolk 2 clothes'/>
    </Clothing>

    <SkinColors>
      <SkinColor value='0' />
      <SkinColor value='1' />
      <SkinColor value='2' />
      <SkinColor value='3' />
      <SkinColor value='4' />
      <SkinColor value='5' />
    </SkinColors>

    <HairColors>
      <Red/>
      <Green/>
      <Black/>
      <Brown/>
    </HairColors>

    <Jobs>
      <Smith/>
      <Cobbler/>
      <Florist/>
    </Jobs>

    <NPCBehaviors>
      <LookAround />
      <Talking />
    </NPCBehaviors>

    <NPCAttributes>
      <Age>
        <!-- 
        <Whitelist>
          
        </Whitelist>
        -->
        <Blacklist>
          <Child/>
          <Old/>
        </Blacklist>
      </Age>
      <Personality/>
    </NPCAttributes>

    <Dialogue>
      <DialogueSegment dialogueSegment='greeting'/>
    </Dialogue>

    <Animations>
      <Animation animationName='human_base0_walkForward' direction='south' animIdx='walk'/>
      <Animation animationName='human_base0_walkBack' direction='north' animIdx='walk'/>
      <Animation animationName='human_base0_walkLeft' direction='west' animIdx='walk'/>
      <Animation animationName='human_base0_walkRight' direction='east' animIdx='walk'/>
    </Animations>

    <StatRanges>
      <Strength min='45' max='55'/>
      <Dexterity min='45' max='55'/>
    </StatRanges>
  </AgentGenerator>
</AgentGenerators>
/----------------------------------------

/----------------------------------------
//! Inheriting Agent Generator XML example
/----------------------------------------
<!-- The inherit attribute allows the new agent generator to define what other agent generator to take its missing pieces from.-->
<AgentGenerators>
  <AgentGenerator Name='Town_Trader'
                  DefaultImage='human_base0'
                  MovementSpeed='2.f'
                  NameSet='VillagerNameSet'
                  inherit='Villager'>

    <Dialogue>
      <DialogueSegment dialogueSegment='TradeDialogue1' />
      <DialogueSegment dialogueSegment='TradeDialogue2' />
      <DialogueSegment dialogueSegment='TradeDialogue4' />
      <DialogueSegment dialogueSegment='TradeDialogue5' />
      <DialogueSegment dialogueSegment='TradeDialogue6' />
      <DialogueSegment dialogueSegment='TradeDialogue7' />
      <DialogueSegment dialogueSegment='TradeDialogue8' />
    </Dialogue>

  </AgentGenerator>
</AgentGenerators>
/----------------------------------------



//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//~Hair Generators
Hair generators take a name, and whether or not it should be considered a default hair generator (aka: if no hair is defined for an agent).
It then defines a set of animations, as well as what colors it supports.

//----------------------
Example XML:
//----------------------
<HairGenerators>
  <HairGenerator name='male_Townsfolk_hair2'
                 dialogueName='Simple Hair'
                 isDefault='true'
                 defautltimage='male_townsfolk_hair2_back0'>
    <Colors>
      <Red value='0' />
      <Brown value='1' />
      <Black value='2'/>
      <Blue value='3' />
      <Green value='5'/>
      <DarkBlue value='6'/>
      <LightRed value='7'/>
    </Colors>

    <Animations>
      <Animation animationName='male_townsfolk_hair2_forward' direction='south' animIdx='walk'/>
      <Animation animationName='male_townsfolk_hair2_back' direction='north' animIdx='walk'/>
      <Animation animationName='male_townsfolk_hair2_left' direction='west' animIdx='walk'/>
      <Animation animationName='male_townsfolk_hair2_right' direction='east' animIdx='walk'/>
    </Animations>
  </HairGenerator>
//----------------------

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//~Item Generators (Clothing Definitions Only)
Presently the only Item type defined is clothing.
The Item generator takes in an item type (Has to be set to clothing), a name, and whether or not it is gender specific.
After which it has two sets of animations it can define per each direction: Animations (male), and AlternativeAnimations (female)

//----------------------
Example XML:
//----------------------
<ItemGenerators>
  <ItemGenerator Name='Townsfolk 2 clothes' DefaultImage='male_townsfolk2_forward0'
                 ItemType='clothes' GenderSpecific='true'>

    <Animations>
      <Animation animationName='male_townsfolk2_forward' direction='south' animIdx='walk'/>
      <Animation animationName='male_townsfolk2_back' direction='north' animIdx='walk'/>
      <Animation animationName='male_townsfolk2_left' direction='west' animIdx='walk'/>
      <Animation animationName='male_townsfolk2_right' direction='east' animIdx='walk'/>
    </Animations>
    <AlternativeAnimations>
      <Animation animationName='female_townsfolk2_forward' direction='south' animIdx='walk'/>
      <Animation animationName='female_townsfolk2_back' direction='north' animIdx='walk'/>
      <Animation animationName='female_townsfolk2_left' direction='west' animIdx='walk'/>
      <Animation animationName='female_townsfolk2_right' direction='east' animIdx='walk'/>
    </AlternativeAnimations>
  </ItemGenerator>
</ItemGenerators>
//----------------------


//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//~Name Generators
A name generator primarily defines a bunch of different lists of names, as well as the order to use each of these lists, per what gender.

//----------------------
Example XML:
//----------------------
<NameGenerators>
  <VillagerNameSet
    male='first_male,last'
    female='first_female,last'>
    <first_male>
      <Alex/>
      <Aaren/>
      <Aiden/>
      <Bill/>
      <Phil/>
      <Ralph/>
      <Will/>
    </first_male>
    <first_female>
      <Amelia/>
      <Ava/>
      <Claire/>
      <Talia/>
      <Tami/>
      <Tamika/>
      <Undine/>
    </first_female>
    <last>
      <Anderson/>
      <Clark/>
      <King/>
      <Smith/>
      <Walker/>
      <Williams/>
    </last>
  </VillagerNameSet>
</NameGenerators>
//----------------------


//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//~NPCJobs
An NPC Job is just a name with a list of stat requirements. This is used in dialogue with an Agent.

//----------------------
example XML:
//----------------------
<NPCJobs>
  <Smith>
    <StatRequirements>
      <Strength value='50'/>
    </StatRequirements>
  </Smith>
</NPCJobs>
//----------------------

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//~NPCAttributes
An NPC Attribute is a key with a bunch of defined possible values for them. They can be called from an agent
generator, and then the values are procedurally generated onto the NPC. It is primarily used for filtering
what agents can be picked to fill the role of a quest specific agent. Agent generators have the option of white listing
or black listing particular values on an NPCAttribute.

//----------------------
example XML:
//----------------------
<NPCAttributes>
  <Age>
    <Child/>
    <Teenager/>
    <Young_Adult/>
    <Adult/>
    <Old/>
  </Age>
</NPCAttributes>
//----------------------


//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//~Agent Stats
Agent stats are a set of integer numbers (value, minimum, maximum), assigned with a name for the stat.
This is used primarily with NPC Jobs, but every agent has them. The values are procedurally generated based on the ranges defined both in the common file,
and on the agent generators themselves. The actual definition for the stats define the true minimum and maximum for after procedural generation.
This is for if the stat is altered later on, that the value will always be clamped inside of the defined range.

//----------------------
example XML:
//----------------------
<AgentStats minimum='0' maximum='100'>
  <Dexterity abbreviation='dex'/>
  <Intelligence abbreviation='int' minimum='0' maximum='100'/> <!-- Default will be 0 to 100, just putting this as an example.-->
</AgentStats>
//----------------------


//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//~Quests
Sub-TOC:
	//! Example Quest XML Data
	//! Quest Variable Declaration Types
	//! Quest Trigger Types
	//! Quest Requirement Types

Quest Declaration Overview:
	In order for a quest to be procedurally generated, a number of variables and events need to be defined.
	First, the variables needed should be predefined (NPCs, Player, and Text references, if nothing else).
	Secondly, the entities that are needed for the quest need to have instructions for how to spawn them.  These instructions will also be used to 
try and grab another pre-existing agent or entity that matches the requirements, if possible.
	Finally, a series of events (with quest requirements and triggers), needs to be defined.

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//! Example Quest XML Data
//--------------------------
<Quests>
  <FindMySibling title='Find My Sibling'>
    <Variables>
      <NPC VariableName='OlderSibling' />
      <NPC VariableName='YoungerSibling' />
      <Player VariableName='player'/>
      <Text VariableName='QuestItem1' 
         value='Staff'/>
      <float VariableName='QuestComplete' 
            defaultValue='0.0' />
    </Variables>
    <AgentsToGenerate>
      <Villager 
        number='1' Job='Smith' 
        VariableName='OlderSibling'
        Dialogue='FindMySiblingStart' 
        AlwaysSpawn='true'>
        <NPCAttributes>
          <Age>
            <WhiteList>
              <Young_Adult />
            </WhiteList>
          </Age>
        </NPCAttributes>
      </Villager>
      <!-- ... -->
    </AgentsToGenerate>
    <Events>
     <Event EventName='Start'>
      <Requirements>
          <Requirement Variable='QuestComplete'
              RequirementType='CheckVariableFloat' 
              value='==0.0'/>
          <Requirement Interact='OlderSibling'
              RequirementType='CheckPlayerInteraction' />
      </Requirements>
      <Trigger>
        <SetQuestVariable value='1.0'
          Variable='QuestComplete' />
        <ChangeDialogue 
          Entity='YoungerSibling'
          DialogueSegment='SiblingSpeech'/>
      </Trigger>
    </Event>
    <!-- ... -->
  </Events>
 </FindMySibling>
</Quests>
//--------------------------


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//! Quest Variable Declaration Types:
//--------------------------
	* Float
		- What this does:
			* Predefines a variable under a given name with a default float value.
		- Example:
			<Float VariableName='QuestComplete' Value='0.0' />

	* Boolean/Bool
		- What this does:
			* Predefines a variable under a given name with a default boolean value.
		- Example
			<Bool VariableName='CompletedQuest' Value='False' />

	* Text
		- What this does:
			* Defines some text to be used by the dialogue system.
		- Example
			<Text VariableName='QuestItem1' Value='Staff' />

	* NPC
		- What this does:
			* Defines a slot for an NPC with a reference name for it.
		- Example
			<NPC VariableName='YoungerSibling' />

	* Feature
		- What this does:
			* Defines a slot for a Feature with a reference name for it.
		- Example
			<Feature VariableName="The door" />

	* Player
		- What this does:
			* Defines a reference name for the player. For a quest to have any access to the player, it must have a player variable defined.
		- Example
			<Player VariableName='Player' />

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//! Quest Trigger Types:
//--------------------------
	* ChangeDialogue
		- What this does:
			* Changes the dialogue of an entity for when the player next speaks to it.
		- Attributes expected:
			* Entity/EntityName - The quest name for the entity that you wish to have's dialogue changed
			* Dialogue/DialogueSegment - The dialogue segment name to set on the entity.
		- Example:
			<ChangeDialogue Entity='YoungerSibling' DialogueSegment='' />

	* CompleteQuests
		- What this does:
			* Takes a quest from the map's or the player's list of quests and moves it into the player's completed list of quests.
		- Attributes expected:
			* no attributes are expected
		- Example:
			<CompleteQuests />

	* GiveEntity
		- What this does:
			* Uses an agent's simplistic inventory to give it a certain amount of a particular item to it.
		- Attributes expected:
			* VariableName/Variable/Name/EntityName/Entity
				-> The name of the quest entity to give an item to.
			* Item/ItemName
				-> Name of the item to give.
			* Value/Amount/AmountToGive
				-> How much of the item to give (0 by default).
		- Example:
			<GiveEntity Variable='Player' ItemName='Staff' Amount='1.0' />
			

	* GlobalSetVariableBool
		- What this does:
			* Sets a global boolean variable to the intended value.
		- Attributes expected:
			* Variable/VariableName 
				-> The name of the global variable to set.
			* Value
				-> What to set the global variable to.
		- Example:
			<GlobalSetVariableBool VariableName='DislikePotatoes' value='true'/>

	* GlobalSetVariableFloat
		- What this does:
			* Sets a global float variable to the intended value.
		- Attributes expected:
			* Variable/VariableName
				-> The name of the global variable to set.
			* Value
				-> What to set the global variable to.
		- Example:
			<GlobalSetVariableFloat VariableName='AwesomeSauce' value='1.0'/>

	* RemoveFromEntity
		- What this does:
			* Uses an agent's simplistic inventory to remove a certain amount of a particular item from it.
		- Attributes expected:
			* VariableName/Variable/Name/EntityName/Entity
				-> The name of the quest entity to give an item to.
			* Item/ItemName
				-> Name of the item to give.
			* Value/Amount/AmountToGive
				-> How much of the item to give (0 by default).
		- Example:
			<RemoveFromEntity Variable='Player' ItemName='Staff' Amount='1.0' />

	* SetQuestVariableFloat
		- What this does:
			* Sets a local quest variable to the intended value.
		- Attributes expected:
			* Variable/VariableName
				-> Name of the quest variable to set
			* Value
				-> Value to set the quest variable to.
			* HowToHandle/Mode/SetMode
				-> Defines whether the value found should be incremented or just set to the numeric value already read.
				-> Increment/Increase - Take the previously defined amount and increase it by the value set.
				-> Set - (this is the default) set the value to the value written on this trigger.
		- Example:
			<SetQuestVariableFloat VariableName='QuestComplete' Value='1.0'/>
			<SetQuestVariableFloat VariableName='GoldReward' Value='-10.0' Mode='Increment'/>

	*SetQuestVariableBool
		- What this does:
			* Sets a local quest variable to the intended boolean value.
		- Attributes expected:
			* Variable/VariableName
				-> Name of the quest variable to set
			* Value
				-> Value to set the quest variable to.
		- Example:
			<SetQuestVariableFloat VariableName='QuestIsDone' Value='false' />

//--------------------------

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//! Quest Requirement types:
//--------------------------
	* CheckEntityHas
		- What this does:
			* All agents have a simplified inventory (names paired with floats). This quest requirement allows for checking a value off of that.
		- Attributes expected:
			* HasItem/Item/HaveItem
				-> The name of the item expected from off of the entity
			* Value
				-> this is what to check the value found against. 
                                -> This is expected to be an operation (>,<,==,<=,>=,!=) followed by a numeric value (1.0)
			* CheckEntity
				-> Takes the name of one of the defined entities in the quest, and uses that to check for the item.
		- Example:
			* <Requirement RequirementType='CheckEntityHas' HasItem='Apple' Value='==1.0' CheckEntity='YoungerSibling'/>

	* CheckVariableBool
		- What this does:
			* Checks a local variable on the quest, provided it is a boolean, and determines if it matches the boolean it is given.
		- Attributes expected:
			* Check/Variable/VariableName
				-> The name of the quest local variable to pull from.
			* Value
				-> This takes in a text version of a boolean (true/false), but it could also have the '==' or '!='
 operator at the start of the text, and still handle it accordingly.
					* examples:
						-> Value='false'
						-> Value='==false'
						-> Value='!=true'
		- Example:
			 <Requirement RequirementType='CheckVariableBool' Variable='PlayerSpookedOnce' Value='==false' />
 
	* CheckVariableFloat
		- What this does:
			* Checks a local variable on the quest, provided it is a float, and determines if it matches the operation and number value it is given.
		- Attributes expected:
			* Check/Variable/VariableName
				-> The name of the quest local variable to pull from.
			* Value
				-> This takes in an operation and a numeric value. It supports all operators (<,>,<=,>=,==,!=).
		- Example:
			 <Requirement RequirementType='CheckVariableFloat' Variable='QuestComplete' Value='==0.0' />

	* GlobalCheckVariableBool
		- What this does:
			* Checks a global variable, provided it is a boolean, and determines if it matches the boolean it is given.
		- Attributes expected:* Check/Variable/VariableName
				-> The name of the global variable to pull from.
			* Value
				-> This takes in a text version of a boolean (true/false), but it could also have the '==' or '!='
 operator at the start of the text, and still handle it accordingly.
					* examples:
						-> Value='false'
						-> Value='==false'
						-> Value='!=true'
		- Example:
			 <Requirement RequirementType='GlobalCheckVariableBool' Variable='PlayerSpookedInAHouse' Value='==false' />

	* GlobalCheckVariableFloat
		- What this does:
			 *Checks a global variable, provided it is a float, and determines if it matches the operation and number value it is given.
		- Attributes expected:
			* Check/Variable/VariableName
				-> The name of the global variable to pull from.
			* Value
				-> This takes in an operation and a numeric value. It supports all operators (<,>,<=,>=,==,!=).
		- Example:
			 <Requirement RequirementType='GlobalCheckVariableFloat' Variable='NumberOfBugsSteppedOn' Value='>=100.0' />

	* CheckPlayerInteraction
		- What this does:
			* Checks which entity the player last interacted with. 
			If the entity expected matches the entity the player last ineracted with, then it returns true.
		- Attributes expected:
			* Interact
				-> The quest name for the entity expected that the player would interact with.
		- Example:
			 <Requirement RequirementType='CheckPlayerInteraction' Interact='YoungerSibling'/>

	* CheckPlayerStats
		- What this does:
			* Takes a list of agent stat values, with an operator (<,>,<=,>=,!=,==), and insures the player meets all of the stat values.
		- Attributes expected:
			* The base node itself does not expect any attributes.
		- Children expected:
			* Each child should have the name of an agent stat for its name.  They should also have an attribute called value 
which defines a numeric value and a number to compare against.
		- Example:
			<Requirement RequiremenType='CheckPlayerStats'>
				<Strength value='>0.0'/>
				<Strength value='<=10.0'/>
			</Requirement>
//--------------------------


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//~Sound
Declaring sounds is really quick and simple: all they need is a file location and a name to reference it by in the game.
However the name of the xml node used is important, as it will sort the sound into either the sound effects or music list depending on it.

So for music, the node needs to have the name "Music".  For sound effects, the node needs to have the name "SoundEffect"

example:
<Sounds>
	<Music filelocation='Data/Music/Matthew Pablo_Shake and Bake.mp3' name='Village1' />
	<SoundEffect filelocation='Data/SoundEffects/SoundEffect.mp3' name='Boom'/>
</Sounds>

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//~SpeechBubbles
As long as the sprite resources are set up, SpeechBubbles are absurdly easy to declare.  All it needs is the sprite resource name.

<SpeechBubbles>
	<SpeechBubble resource='SpeechBubble'/>
</SpeechBubbles>

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//~DialogueSegments
Sub-TOC:
	//! Several Example Dialogue Segments
	//! Dialogue Triggers/Actions
	//! Dialogue Choices
	//! Common Dialogue Variable Names
	

Dialogue segments take a name, a speechbubble, and a bunch of text. The text is split into individual words at the start,
and re-strung together whenever the dialogue segment is openned. Variables can be used in the dialogue in a few select cases.
Also, dialogue segments can define a series of choices for the player, and also define if it is a part of a unique group or not.

//----------------------------------------------
//! Several Example Dialogue Segments
//----------------------------------------------
<DialogueSegments SpeechBubble='SpeechBubble'>
  <!-- Dialogue segments can define text via attributes or child text nodes. All of the values get added together via order encountered. -->
  <Greeting4 uniqueGrouping="greeting" text="Hello my name is $MyFirstName$, and I am a $Job$. I love it!" />

  <LongWindedVillagerGreeting>
    <Text text="Hello, I am $MyFirstName$."/>
    <Text text=" I love my job as a $Job$."/>
    <Text text=" I still wish I could spend more time with my family and friends though."/>
  </LongWindedVillagerGreeting>
</DialogueSegments>
//----------------------------------------------

//----------------------------------------------
//! Dialogue Triggers/Actions

Triggers are defined through another type of child node for the dialogue segment. The triggers will activate upon the dialogue segment closing.

There are only two triggers for dialogue, and those are GlobalSetVariableBool, and GlobalSetVariableFloat. They behave the same way as the quest version of these.

Example:
<DialogueSegments>
  <Greeting1 uniqueGrouping="greeting" text="Hello my name is $MyFirstName$.">
    <Triggers>
      <GlobalSetVariableFloat Variable="Greeting1Encountered" value="1.0"/>
    </Triggers>
  </Greeting1>
</DialogueSegments>


//----------------------------------------------

//----------------------------------------------
//! Dialogue Choices
Dialogue choices open once the regular set of text has been fully read by the player. From there the player can select a dialogue choice.  The dialogue choice
can define another dialogue segment to open as well as a set of dialogue requirements, and dialogue triggers. The dialogue requirements will hide 
the choice if not all requirements are met. The triggers will happen occur upon the player selecting the choice.

Supported Dialogue Requirements:
	CheckGlobalVariableBool - operates similarly to the quest variant of a similar name.
	CheckGlobalVariableFloat - operates similarly to the quest variant of a similar name.
	NPCHasAgentStatValue - checks a given stat on the speaking NPC.
			- Attributes:
				* Variable/VariableName - The name of the stat to check.
				* Value - the operator (<,>,<=,>=,==,!=) and interger number pair to compair the stat to.
			- Example <NPCHasAgentStatValue VariableName='Strength' value='>10' />
	NPCHasAttributeValue - Takes an NPCAttribute name value pair, and checks if the NPC has it.
			- Attributes:
				* Name/AttributeName = The name of the attribute to check.
				* Value/AttributeValue = The value of the attribute wanted.
			- Example <NPCHasAttributeValue AttributeName='Age' AttributeValue='Old' />
	PlayerHasAgentStatValue - Checks a stat on the player, and verifies that it meets the requirement.
			- Attributes:
				* VariableName - The name of the stat to check.
				* Value - The operator (>,<,>=,<=,==,!=) and integer value pair to check the stat against.
			- Example: <PlayerHasAgentStatValue VariableName='Strength' Value='>10' />

Example:
<DialogueSegments>
  <LongDialogueExampleWithChoice>
    <Text text="Hey, did you know monsters are coming back?"/>
    <Decision>
      
      <Yes1 DialogueToOpen="LongDialogueExampleWithChoiceSelectionYes">
        <Requirements>
          <GlobalCheckVariableFloat Variable="Choice1" value="==0.0"/>
        </Requirements>
        <Triggers>
          <GlobalSetVariableFloat Variable="Choice1" value="1.0"/>
        </Triggers>
      </Yes1>
      
      <Yes2 DialogueToOpen="LongDialogueExampleWithChoiceSelectionYes">
        <Requirements>
          <GlobalCheckVariableFloat Variable="Choice1" value="==1.0"/>
        </Requirements>
        <Triggers>
          <GlobalSetVariableFloat Variable="Choice1" value="0.0"/>
        </Triggers>
      </Yes2>
      
      <No DialogueToOpen="LongDialogueExampleWithChoiceSelectionNo"/>
      <Maybe DialogueToOpen="LongDialogueExampleWithChoiceSelectionMaybe"/>
    </Decision>
  </LongDialogueExampleWithChoice>
</DialogueSegments>

//----------------------------------------------

//----------------------------------------------
//! Common Dialogue Variable Names
//----------------------------------------------
For general use:
	$PlayerName$ - Uses the player's name (which is set to "Player").
	$Job$ - Returns the NPC being talked to's job.
	$MyName$ - Returns the full name of the NPC being talked to.
	$MyFirstName$ - Returns the first name of the NPC.
	$MySecondName$ - Returns the second name of the NPC.
	$MyLastName$ - Returns the last name of the NPC.

For use with quest variables:
	$[Quest Title].[NPC quest name].Name$ - returns the full name of the NPC.
	$[Quest Title].[NPC quest name].FirstName$ - returns the first name of the NPC.
	$[Quest Title].[NPC quest name].SecondName$ - returns the second name of the NPC.
	$[Quest Title].[NPC quest name].SiblingTitle$ - "Brother/Sister" based off the gender of the NPC.
	$[Quest Title].[NPC quest name].GenderTitle$ - "Male/Female" based off the gender of the NPC.
	$[Quest Title].[NPC quest name].GenderPronounOwning$ - "His/Her" is returned based on the gender of the NPC.
	$[Quest Title].[NPC quest name].GenderPronounIs$ - "He/She" is returned based on the gender of the NPC.
//----------------------------------------------

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//~DialogueGroupings
A dialogue segment can declare itself apart of a dialogue group.  If that dialogue group was not uniquely created at that point, then it will be.
Dialogue groups can also be declared in XML, and by doing so it is possible to set dialogue triggers for it,
such that whenever a member of it's group is called, the triggers are used.
Dialogue group names can take the place of dialogue segments on NPCs. (This allows the npc to use all of the members of the dialogue group; and leads to procedural
generation of which one to use).

With the addition of dialogue groups, dialogue segments now have the ability to use dialogue requirements.  The dialogue requirements prevent the dialogue group
from selecting them if the dialogue segments requirements are not met.

Examples:

//Dialogue segment declaring itself as a part of a group, via the "uniqueGrouping" attribute:
//----------------------------------------------
<DialogueSegments>
  <Greeting1 uniqueGrouping="greeting" text="Hello my name is $MyFirstName$.">
</DialogueSegments>
//----------------------------------------------

//Declaring a dialogue group through its own XML:
//----------------------------------------------
<DialogueGroupings>
  
  <!-- The name of the nodes at this level should be the name of the group. -->
  <Greeting>
    <Triggers>
      <GlobalSetVariableFloat Variable="GreetingEncountered" value="1.0"/>
    </Triggers>
  </Greeting>
</DialogueGroupings>
//----------------------------------------------


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//~MapData

The map data file is able to define for a map:
	* what music plays on the map
	* Whether or not the camera is constrained to the map's bounds.
	* A bunch of tile events per each tile.
	* What features to generate.
	* What agents to generate.
	* What quests to generate.

It is also possible to define a set of unique positions for each agent, feature and quest entities (by name).

//---------------------------------------------
Example XML:
//---------------------------------------------
<MapData Music='Village1' ConstrainCameraBounds='true'>
  <TileEvents>
    <TileEvent TileCoords='19,10'>
      <OnEnter Function='ChangeMaps' MapFile='Map1Test' PlayerPosition='1,10'/>
    </TileEvent>
    <TileEvent TileCoords='9,19'>
      <OnEnter Function='ChangeMaps' MapFile='TradeSquare' PlayerPosition='29,1'/>
    </TileEvent>
  </TileEvents>

  <FeaturesToGenerate>
    <Door numbertospawn='3'> <!-- Attributes for single spawn, children for multi-spawn. -->
      <Position position='7,9'/>
      <Position position='14,1'/>
      <Position position='14,14'/>
    </Door>
    <Firepit>
      <Position position='1,3'/>
    </Firepit>
  </FeaturesToGenerate>

  <AgentsToGenerate>
    <Villager number='1' Job='Smith' chance='0.5'/>
    <Villager number='3-5'/>
  </AgentsToGenerate>
  
  <Quests>
    <Quest QuestTitle='FindMySibling' PercentChanceToOccur='1.0'>
      <Default>
        <Positions forEntity='OlderSibling'>
          <Position position='12,15'/>
        </Positions>
        <Positions forEntity='YoungerSibling'>
          <Position position='4,15'/>
          <Position position='7,11'/>
          <Position position='15,3'/>
        </Positions>
      </Default>
    </Quest>
  </Quests>
</MapData>
//---------------------------------------------

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//~MapFiles

MapFiles are just an associated map name, image and map data XML file.

//---------------------------------------------
Example XML:
//---------------------------------------------
<MapFiles>
  <Default DialogueName='Swamp Town'
    image='Data/Maps/TestMap.png' mapdata='Data/XML/MapData/MapData.TestMap.xml'/>
</MapFiles>
//---------------------------------------------

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//~How to make new Dialogue Requirements
This requires having access to the code for the asset.
In order to create a new Dialogue Requirement, a new class for the intended requirement must be made.
This new class will need to inherit from "DialogueRequirement" found under "Game/Dialogue/DialogueRequirements/DialogueRequirement.hpp"
It will also need to create a static "DialogueRequirementRegistrationHelper" which is found under "Game/Dialogue/DialogueRequirements/DialogueRequirementRegistrationHelper.hpp"

Requiremented Constructors:
	ExampleDialogueRequirement();
	ExampleDialogueRequirement(const XMLNode& node);
	ExampleDialogueRequirement(const DialogueRequirement& other);
	virtual ~ExampleDialogueRequirement();

The first constructor is just the default; just declaring it will be fine.
The second constructor takes an XMLNode, and needs to read the attributes and children of that node for any important information.
The third constructor is the copy constructor, it should copy any necessary information.

Inherited Methods:
each Dialogue Requirement variant must implement the following methods:
	virtual void WriteDialogueTriggerToString(std::string& str, int indentationAmt) const override;
	virtual bool CheckIfRequirementIsMet() const override;
	virtual DialogueRequirement* Clone() const override;

WriteDialogueTriggerToString spits out the information for the requirement to the debug log.
CheckIfRequirementIsMet is where the actual check occurs.
Clone must return a new'd version of the DialogueRequirement variant, such that it copies the information read in from XML.
aka it should do this:
	return new ExampleDialogueRequirement(*this);

Registration Helper:
The registration helper takes a name, plus two function pointers. These function pointers both return a DialogueRequirement*, just one takes nothing while the other
takes a const XMLNode& node);
	//HPP
	#define STATIC
	DialogueRequirement* DialogueReqExampleCreationFunc(const XMLNode& node);
	DialogueRequirement* DialogueReqExampleDefaultCreationFunc();
	class ExampleDialogueRequirement
	{
		//...
	protected:
		static const DialogueRequirementRegistrationHelper s_ExampleDialogueRequirementRegHelper;
		//...
	}

	//CPP
	//Static Variables
	STATIC const DialogueRequirementRegistrationHelper ExampleDialogueRequirement::s_ExampleDialogueRequirementRegHelper
		= DialogueRequirementRegistrationHelper("ExampleDialogueRequirement", DialogueReqExampleCreationFunc, DialogueReqExampleDefaultCreationFunc);

Example Dialogue Requirement:
//------------------
//-------------
// HPP
#include "Game/Dialogue/DialogueRequirements/DialogueRequirement.hpp"
#include "Game/Dialogue/DialogueRequirements/DialogueRequirementRegistrationHelper.hpp"

#define STATIC
//Spawning Functions
DialogueRequirement* DialogueReqExampleCreationFunc(const XMLNode& node);
DialogueRequirement* DialogueReqExampleDefaultCreationFunc();

class ExampleDialogueRequirement : public DialogueRequirement
{
private:
protected:
	//Static Variables
	static const DialogueRequirementRegistrationHelper s_ExampleDialogueRequirementRegHelper;

	//Variables
public:
	//Constructors
	ExampleDialogueRequirement();
	ExampleDialogueRequirement(const XMLNode& node);
	ExampleDialogueRequirement(const ExampleDialogueRequirement& other);
	virtual ~ExampleDialogueRequirement();

	//Operations
	virtual void WriteDialogueTriggerToString(std::string& str, int indentationAmt) const override;
	virtual bool CheckIfRequirementIsMet() const override;
	virtual DialogueRequirement* Clone() const override;
}
//-------------
//CPP
#include "Game/Common/GameDebuggingCommons.hpp" //Include this for debug printing functions.

//Spawning Functions
DialogueRequirement* DialogueReqExampleCreationFunc(const XMLNode& node)
{
	return new ExampleDialogueRequirement(node);
}
DialogueRequirement* DialogueReqExampleDefaultCreationFunc()
{
	return new ExampleDialogueRequirement();
}

//Static Variables
STATIC const DialogueRequirementRegistrationHelper ExampleDialogueRequirement::s_ExampleDialogueRequirementRegHelper
	= DialogueRequirementRegistrationHelper("ExampleDialogueRequirement", DialogueReqExampleCreationFunc, DialogueReqExampleDefaultCreationFunc);

//Constructors
ExampleDialogueRequirement::ExampleDialogueRequirement()
	: DialogueRequirement()
{
}
ExampleDialogueRequirement::ExampleDialogueRequirement(const XMLNode& node)
	: DialogueRequirement(node)
{
	for(int attrIdx = 0; attrIdx < node.nAttributes(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
	}
	for(int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
	}
}
ExampleDialogueRequirement::ExampleDialogueRequirement(const ExampleDialogueRequirement& other)
	: DialogueRequirement(other)
{
}

//Operations
void ExampleDialogueRequirement::WriteDialogueTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Dialogue Requirement Name: Example Dialogue Requirement\n";
}

bool ExampleDialogueRequirement::CheckIfRequirementIsMet() const
{
	return false;
}

DialogueRequirement* ExampleDialogueRequirement::Clone() const
{
	return new ExampleDialogueRequirement(this);
}
//------------------

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//~How to make new Quest Requirements
The core concept is the same as with Dialogue Requirements, see "How to make new Dialogue Requirements";
	but replace "DialogueRequirement" with "QuestRequirement",
	and replace "DialogueRequirementRegistrationHelper" with "QuestRequirementRegistration".

includes for Quest Requirement:
	#include "Game/Quest/QuestEvents/QuestRequirement.hpp"
	#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementRegistration.hpp"
	#include "Game/Quest/Quest.hpp" //This is for if need access to variables on the owning quest.

Constructor Examples:
	ExampleQuestRequirement();
	ExampleQuestRequirement(const XMLNode& node);
	ExampleQuestRequirement(const ExampleQuestRequirement& other,
		Quest* owningQuest = nullptr);
	virtual ~ExampleQuestRequirement();

The only key difference between DialogueRequirement and QuestRequirement inheritance is the copy constructor now has an owning quest variable.
Just handle it like this in the CPP:
ExampleQuestRequirement::ExampleQuestRequirement(const ExampleQuestRequirement& other, Quest* owningQuest)
	: QuestRequirement(other, owningQuest)

Inherited Operations:
	virtual void WriteQuestRequirementToString(std::string& str, int indentationAmt) const override;
	virtual const bool DeterminePlayerMeetsQuestRequirement() const override;
	virtual QuestRequirement* Clone(Quest* owningQuest = nullptr) const override;
Clone operator should look like this:
	QuestRequirement* ExampleQuestRequirement::Clone(Quest* owningQuest) const
	{
		return new ExampleQuestRequirement(*this, owningQuest);
	}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//~How to make new Dialogue Triggers
The general concept is the same as Dialogue Requirements, but has some different operations.
See "How to make new Dialogue Requirements".

Replace DialogueRequirement with QuestTrigger and DialogueRequirementRegistrationHelper with DialogueActionRegistrationHelper

includes for Dialogue Triggers:
	#include "Game/Dialogue/DialogueActions/DialogueTrigger.hpp"
	#include "Game/Dialogue/DialogueActions/DialogueActionRegistrationHelper.hpp"

Constructor Examples:
	ExampleDialogueTrigger();
	ExampleDialogueTrigger(const XMLNode& node);
	ExampleDialogueTrigger(const ExampleDialogueTrigger& other);
	virtual ~ExampleDialogueTrigger();

Inherited Operations:
	virtual void WriteDialogueTriggerToString(std::string& str, int indentationAmt) const override;
	virtual void TriggerAction() override;
	virtual DialogueTrigger* Clone() const override;

TriggerAction is where the action is actually performed.

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//~How to make new Feature Behaviors
Some of the general concepts for Feature Behaviors matches with Dialogue Requirements;
See "How to make new Dialogue Requirements" first.
Replace DialogueRequirement with FeatureBehavior, and DialogueRequirementRegistrationHelper with FeatureBehaviorRegistrationHelper.

Constructor Examples:
	ExampleFeatureBehavior();
	ExampleFeatureBehavior(const XMLNode& node);
	ExampleFeatureBehavior(const ExampleFeatureBehavior& other);
	virtual ~ExampleFeatureBehavior();

Note, that unlike other Constructors, a "DefinedFeatureBehaviorIdxes" must be given for the first two when passing to FeatureBehavior.
This is as Feature Behaviors could operate on update, placement, or interact.
This is how to set up the main two constructors:
ExampleFeatureBehavior::ExampleFeatureBehavior()
	: FeatureBehavior(INTERACTIVE_FEATURE_BEHAVIOR) //this is an interactive Behavior
	//FeatureBehavior(ON_PLACEMENT_FEATURE_BEHAVIOR) //this means the Behavior will only occur on placement
	//FeatureBehavior(ON_UPDATE_FEATURE_BEHAVIOR) //this means the behavior will run on update.
{
}

ExampleFeatureBehavior::ExampleFeatureBehavior(const XMLNode& node) 
	: FeatureBehavior(node, INTERACTIVE_FEATURE_BEHAVIOR)
{
}

Pre existing DefinedFeatureBehaviorIdxes:
	INVALID_FEATURE_BEHAVIOR = -1, //Do not use this, Feature behavior will not be added.
	ON_PLACEMENT_FEATURE_BEHAVIOR,
	INTERACTIVE_FEATURE_BEHAVIOR,
	ON_UPDATE_FEATURE_BEHAVIOR,
	NUMBER_OF_DEFIND_FEATURE_BEHAVIOR_TYPES //Do not use this, Feature behavior will not be run.

Note it is possible to add more DefinedFeatureBehaviorIdxes, but it will require going into the Feature class and telling it how to handle that new type of
Feature Behaviors.

Inherited Operations
	virtual int WriteFeatureBehaviorToString(std::string& str, int indentationLevel) const;
	virtual void CheckMapNodeForAttributesOfInterest(const XMLNode& node);
	virtual void TriggerOnPlacement(const Map* map);
	virtual bool EntityInteractWithFeature(BaseGameEntity* entity);
	virtual void UpdateFeature(float dt);
	virtual FeatureBehavior* Clone() = 0;

CheckMapNodeForAttribtuesOfInterest allows for reading special information for when being placed on the map.
As an example, we could tell a feature with a teleport behavior to specifically teleport to one tile some place else on the map.

TriggerOnPlacement is run for ON_PLACEMENT_FEATURE_BEHAVIOR types, and only occurs once the Feature is officially placed.

EntityInteractWithFeature is run for the INTERACTIVE_FEATURE_BEHAVIOR types, when an entity (really only the player) interacts with the feature.

UpdateFeature runs each frame for the ON_UPDATE_FEATURE_BEHAVIOR types.

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//~How to make new NPC Behaviors
Some of the general concepts for NPC Behaviors matches with Dialogue requirements;
See "How to make new Dialogue Requirements" first.
Replace DialogueRequirement with NPCBehavior, and DialogueRequirementRegistrationHelper with NPCBehaviorRegistrationHelper.

Includes: 
	#include "Game/GameEntities/Agents/NPCs/NPCBehavior.hpp" //NPCBehaviorRegistrationHelper is included with this one, but just in case:
	#include "Game/GameEntities/Agents/NPCs/NPCBehaviorRegistrationHelper.hpp"

Constructor Examples:
	ExampleNPCBehavior();
	ExampleNPCBehavior(const XMLNode& node);
	ExampleNPCBehavior(const ExampleNPCBehavior& other);
	virtual ~ExampleNPCBehavior();

Inerhitted Operations:
	virtual void CleanUpOnUpdate() override;
	virtual int WriteNPCBehaviorToString(std::string& str, int indentationLvl) const override;
	virtual NPCBehavior* Clone() override;
	virtual const float CalcUtility() override;
	virtual void Run(float dt, bool ranLastFrame) override;

CalcUtility returns a float; if the number is higher than the other NPCBehavior utility scores, than this particular NPCBehavior will run.
CleanUpOnUpdate runs whenever this behavior is no longer going to run on the new frame (has a lower score than another NPCBehavior, when it ran last frame).
Run performs the Behavior.

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//~How to make new Quest Triggers
The general concept is the same as Dialogue Requirements and Quest Requirements, has some different operations.
See "How to make new Dialogue Requirements" and "How to make new Quest Requirements".

Replace DialogueRequirement with QuestTrigger, and DialogueRequirementRegistrationHelper with QuestTriggerRegistration.

Includes:
	#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerRegistration.hpp"
	#include "Game/Quest/QuestEvents/QuestTrigger.hpp"
	#include "Game/Quest/Quest.hpp" //This is for if need access to variables on the owning quest.

Constructor Examples:
	ExampleQuestTrigger();
	ExampleQuestTrigger(const XMLNode& node);
	ExampleQuestTrigger(const ExampleQuestTrigger& other, Quest* owningQuest = nullptr);
	virtual ~ExampleQuestTrigger();

Inherited Operations:
	virtual bool PerformQuestTrigger() override;
	virtual QuestTrigger* Clone(Quest* owningQuest = nullptr) const override;
	virtual void WriteQuestTriggerToString(std::string& str, int indentationAmt) const override;