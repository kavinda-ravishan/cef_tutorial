<script setup lang="ts">
import { ref } from 'vue'
const darkMode = ref(true)
</script>

<template>
    <v-app :theme="darkMode ? 'dark' : 'light'">
        <v-main>
            <v-container width="800">
                <v-card class="mb-3 py-2 px-3">
                    <p>
                        A web browser, often abbreviated as browser, is an application for accessing websites. When a user requests a web page from a particular website, the browser retrieves its files from a web server and then displays the page on the user's screen. Browsers can also display content stored locally on the user's device.
                    </p>
                    <p>
                        The purpose of a web browser is to fetch content and display it on the user's device.[5] This process begins when the user inputs a Uniform Resource Locator (URL), such as https://en.wikipedia.org/, into the browser's address bar. Virtually all URLs on the Web start with either http: or https: which means they are retrieved with the Hypertext Transfer Protocol (HTTP). For secure mode (HTTPS), the connection between the browser and web server is encrypted, providing a secure and private data transfer.[6] For this reason, a web browser is often referred to as an HTTP client[7][8] or a user agent. Requisite materials, including text, style sheets, images, and other types of multimedia, are downloaded from the server. Once the materials have been downloaded, the web browser's engine (also known as a layout engine or rendering engine) is responsible for converting those resources into an interactive visual representation of the page on the user's device.[9] Modern web browsers also contain separate JavaScript engines which enable more complex interactive applications inside the browser.[10] A web browser that does not render a graphical user interface is known as a headless browser.
                    </p>
                </v-card>
                <v-card class="mb-3 py-2 px-3">
                    <p>
                        Web pages usually contain hyperlinks to other pages and resources. Each link contains a URL, and when it is clicked or tapped, the browser navigates to the new resource. Most browsers use an internal cache of web page resources to improve loading times for subsequent visits to the same page. The cache can store many items, such as large images, so they do not need to be downloaded from the server again. Cached items are usually only stored for as long as the web server stipulates in its HTTP response messages.[11][12]
                    </p>
                </v-card>
                <v-switch 
                    v-model="darkMode"
                    label="Dark Mode"
                ></v-switch>
            </v-container>
        </v-main>
    </v-app>
</template>

<style scoped>
</style>
