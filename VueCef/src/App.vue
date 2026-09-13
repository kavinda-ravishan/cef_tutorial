<script setup lang="ts">
import { ref, computed } from 'vue'

interface Item {
    name: string;
    price: number;
}
interface Entry {
    item: Item;
    quantity: number;
}

const inventory: Item[] = [
    { name: 'Pepe\'s Existential Dread Poster', price: 14.00 },
    { name: 'Rickroll Remixes Vol. 420', price: 12.25 },
    { name: 'No Nut November Nutella', price: 6.50 },
    { name: 'Zoom Call Pajama Pants', price: 18.75 },
    { name: 'NPC Brainiac Gaming Chair', price: 149.00 },
    { name: 'Tide Pod Cereal Crunch', price: 4.25 },
    { name: 'Discord Mod Tears Spray', price: 8.50 },
    { name: 'HR\'s Quiet Firing Manual', price: 7.75 },
    { name: 'Twitch Streamer Rage Quit Button', price: 19.00 },
    { name: 'Dumpster Fire Candle Set', price: 29.25 },
    { name: 'Elon\'s Cryo-Bricked Bitcoin Miner', price: 49.50 },
    { name: 'Musk\'s Mars Colony Bond Scam', price: 14.75 },
    { name: 'Nigerian Prince Scam Starter Kit', price: 9.00 },
    { name: '420 Blaze It Cufflinks', price: 24.25 },
    { name: 'Karens of Twitter Anti-Block Spray', price: 7.50 },
    { name: 'Flat Earth Society Globe Eraser', price: 5.75 },
    { name: 'Crying Elon Meme Tissue Pack', price: 3.00 },
    { name: 'Area 51 Alien Adoption Papers', price: 12.25 },
    { name: 'Guinea Pig Therapist Hotline Subscription', price: 4.50 },
    { name: 'Dumpster Fire Rodeo Clown Costume', price: 49.75 },
]

const entries = ref<Entry[]>([])

function addItem() {
    entries.value.push({item: inventory[0]!, quantity: 1})
}

function removeItem(i: number) {
    entries.value.splice(i, 1)
}

const grandTotal = computed(() => 
    entries.value.reduce((sum, entry) => sum + entry.item.price * entry.quantity, 0)
)

const cef_sync_func_ret_val = ref('null')
const cef_async_func_ret_val = ref('null')

function callCefSyncFunc() {
    interface NanoCefAPI {
        SyncCefJSFunc(text: string): boolean;
    }

    console.log('Cef Sync Function called')

    const nanoCefApi = window as unknown as NanoCefAPI;

    try {
        const ret_val: boolean = nanoCefApi.SyncCefJSFunc(" --- JS Sync string --- ");
        cef_sync_func_ret_val.value = (ret_val ? 'YES' : 'NO')
    } catch(error) {
        if (error instanceof Error) {
            cef_sync_func_ret_val.value = error.message;
        } else {
            cef_sync_func_ret_val.value = String(error);
        }
    }
}

function callCefAsyncFunc() {
    interface NanoCefAPI {
        AsyncCefJSFunc(
            text: string, 
            acceptFunc: (result: boolean) => void, 
            rejectFunc: (errorMessage: string) => void, 
        ): void;
    }

    console.log('Cef Async Function called')

    const nanoCefApi = window as unknown as NanoCefAPI;

    try {
    nanoCefApi.AsyncCefJSFunc(
        " --- JS string --- ", 
        (result: boolean) => { cef_async_func_ret_val.value = result ? "YES" : "NO" }, 
        (errorMessage: string) => { cef_async_func_ret_val.value = errorMessage }
    );
    } catch(error) {
        if (error instanceof Error) {
            cef_async_func_ret_val.value = error.message;
        } else {
            cef_async_func_ret_val.value = String(error);
        }
    }

}

const powerLevel = ref<number | null>(null)
const powerLevelNorm = computed(() => powerLevel.value ?? 0)
const inProgess = ref<boolean>(true)

function adjustPowerLevel() {
    interface NanoCefAPI {
        CefPowerLevel(
            text: string, 
            acceptFunc: (result: boolean) => void, 
            rejectFunc: (errorMessage: string) => void, 
        ): void;
    }

    const nanoCefApi = window as unknown as NanoCefAPI;

    inProgess.value = true
    powerLevel.value = 0

    // nanoCefApi.CefPowerLevel(
    //     "Question 1",
    //     btn => { 
    //         powerLevel.value = btn ? <number>powerLevel.value + 1 : <number>powerLevel.value 
    //         nanoCefApi.CefPowerLevel(
    //             "Question 2",
    //             btn => { 
    //                 powerLevel.value = btn ? <number>powerLevel.value + 1 : <number>powerLevel.value 
    //                 nanoCefApi.CefPowerLevel(
    //                     "Question 3",
    //                     btn => { 
    //                         powerLevel.value = btn ? <number>powerLevel.value + 1 : <number>powerLevel.value 
    //                         inProgess.value = false
    //                     }, 
    //                     msg => { inProgess.value = false; powerLevel.value = null }
    //                 )
    //             }, 
    //             msg => { inProgess.value = false; powerLevel.value = null }
    //         )
    //     }, 
    //     msg => { inProgess.value = false; powerLevel.value = null }
    // )

    function powerLevelPromise(text: string): Promise<boolean> {
        return new Promise<boolean>((accept, reject) => {nanoCefApi.CefPowerLevel(text, accept, reject)})
    }

    function quest(text: string): Promise<void> {
        return powerLevelPromise(text).then((btn) => { powerLevel.value = btn ? <number>powerLevel.value + 1 : <number>powerLevel.value })
    }

    quest("Question 1")
    .then(() => quest("Question 2"))
    .then(() => quest("Question 3"))
    .catch(() => {powerLevel.value = null})
    .finally(() => {inProgess.value = false})
}

</script>

<template>
    <v-app>
        <v-main>
            <v-container width="1024">
                <v-card class="mb-3 pa-3">
                    <h2 class="mb-2 ml-4">My Shopping Cart</h2>
                    <v-row v-for="(entry, i) in entries" align="center">
                        <v-col cols="1">
                            <v-btn 
                                icon="mdi-trash-can" 
                                size="small" 
                                variant="text"
                                color="red"
                                @click="removeItem(i)"
                            ></v-btn>
                        </v-col>
                        <v-col cols="7">
                            <v-autocomplete
                                color="primary"
                                variant="outlined"
                                density="compact"
                                hide-details
                                :items="inventory"
                                item-title="name"
                                v-model="entry.item"
                                return-object
                            >
                            </v-autocomplete>
                        </v-col>
                        <v-col cols="2">
                            <v-number-input
                                color="primary"
                                variant="outlined"
                                density="compact"
                                hide-details
                                control-variant="stacked"
                                v-model="entry.quantity"
                                :min="1"
                            ></v-number-input>
                        </v-col>
                        <v-col cols="2" class="d-flex justify-end">
                            <p class="price">${{ (entry.item.price * entry.quantity).toFixed(2) }}</p>
                        </v-col>
                    </v-row>
                    <v-row align="center" dense>
                        <v-col cols="10" class="d-flex justify-end">
                            <p class="price" style="padding-right: 0;">Grand Total:</p>
                        </v-col>
                        <v-col cols="2" class="d-flex justify-end">
                            <p class="price">${{ grandTotal.toFixed(2) }}</p>
                        </v-col>
                    </v-row>
                    <div class="d-flex justify-end ma-5">
                        <v-btn icon="mdi-plus" color="purple" @click="addItem"></v-btn>
                    </div>
                </v-card>
                <v-card class="mb-3 pa-3">
                    <h2 class="mb-2 ml-4">Call CEF C++ functions from JS</h2>
                    <div class="d-flex align-center">
                        <v-btn color="purple" @click="callCefSyncFunc">Call CEF Sync Function</v-btn>
                        <p class="ml-5">CEF Sync Function Return Value : {{ cef_sync_func_ret_val }}</p>
                    </div>
                    <div class="d-flex align-center">
                        <v-btn color="purple" @click="callCefAsyncFunc">Call CEF Async Function</v-btn>
                        <p class="ml-5">CEF Async Function Return Value : {{ cef_async_func_ret_val }}</p>
                    </div>
                </v-card>
                <v-card title="Power Level" class="mb-3 pa-3">
                    <v-progress-linear
                        v-model="powerLevelNorm"
                        height="20"
                        color="blue-darken-3"
                        :indeterminate="powerLevel === null"
                        :stream="inProgess"
                        rounded
                        max="3"
                    >
                    </v-progress-linear>
                    <div class="d-flex justify-center mt-10 mb-5">
                        <v-btn color="purple" size="x-large" @click="adjustPowerLevel">Adjust Power Level</v-btn>
                    </div>
                </v-card>
            </v-container>
        </v-main>
    </v-app>
</template>

<style scoped>
.price {
    font-size: 20px;
    font-weight: bold;
    padding-right: 10px;
    margin: 0;
}
</style>
